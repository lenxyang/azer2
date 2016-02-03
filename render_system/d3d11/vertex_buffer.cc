#include "azer/render_system/d3d11/vertex_buffer.h"

#include <sstream>
#include <windows.h>
#include "base/logging.h"
#include "base/strings/string_util.h"
#include "azer/base/string.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/hlsl_compile.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"

namespace azer {
namespace d3d11 {

// class D3DVertexLayout
D3DVertexLayout::D3DVertexLayout(VertexDesc* desc) 
    : VertexLayout(desc), 
      input_layout_(NULL) {
}

D3DVertexLayout::~D3DVertexLayout() {
  SAFE_RELEASE(input_layout_);
}

std::string D3DVertexLayout::GenVSForDesc(VertexDesc* vertex_desc) {
  std::stringstream ss;
  ss << "\nstruct VSInput {\n";
  for (int32 i = 0; i < vertex_desc->element_count(); ++i) {
    const VertexDesc::Desc* desc = vertex_desc->descs() + i;
     std::string name(desc->name);
     ss << HLSLTypeName(desc->type) << " "
        << ::base::StringToLowerASCII(name) << ":" 
        << StringToUpperASCII(name) << desc->semantic_index
        << ";" << std::endl;
  }
  ss << "};\n";
  ss << "struct VSOutput {float4 position: SV_POSITION;};\n"
     << "VSOutput vs_main(VSInput input) {"
     << "VSOutput o; o.position = float4(0, 0, 0, 0);return o;}\n";
  return ss.str();
}

bool D3DVertexLayout::Init(RenderSystem* rs) {
  std::string shader = GenVSForDesc(vertex_desc());
  std::string msg;
  D3DBlobPtr blob(CompileShaderForStage(kVertexStage, shader, "gen", &msg));
  CHECK(blob.get()) << "compile shader failed." << shader << "\n" << msg;
  return Init(rs, blob.get());
}

void D3DVertexLayout::CreateInputDesc(VertexDesc* vertex_desc, 
                                      D3D11_INPUT_ELEMENT_DESC* d3ddesc) {
  const VertexDesc::Desc* desc = vertex_desc->descs();
  DCHECK_LT(vertex_desc->element_count(), kMaxInputElementDesc);
  D3D11_INPUT_ELEMENT_DESC* curr_layout = d3ddesc;
  for (int i = 0; i < vertex_desc->element_count(); ++i, ++curr_layout, ++desc) {
    curr_layout->SemanticName = desc->name;
    curr_layout->SemanticIndex = desc->semantic_index;
    curr_layout->Format = TranslateFormat(desc->type);
    curr_layout->InputSlot = desc->input_slot;
    curr_layout->AlignedByteOffset = vertex_desc->offset(i);
    curr_layout->InstanceDataStepRate = desc->instance_data_step;
    if (desc->instance_data_step == 0) {
      curr_layout->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    } else {
      curr_layout->InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    }
  }
}

bool D3DVertexLayout::ValidateShaderLayout(RenderSystem* rs, ID3DBlob* blob) {
  HRESULT hr;
  DCHECK(typeid(*rs) == typeid(D3DRenderSystem));
  D3DRenderSystem* render_system = static_cast<D3DRenderSystem*>(rs);
  ID3D11Device* d3d_device = render_system->GetDevice();
  D3D11_INPUT_ELEMENT_DESC d3ddesc[kMaxInputElementDesc] = {0};
  CreateInputDesc(desc_, d3ddesc);
  hr = d3d_device->CreateInputLayout(d3ddesc,
                                     desc_->element_count(),
                                     (blob ? blob->GetBufferPointer() : NULL),
                                     (blob ? blob->GetBufferSize() : NULL),
                                     NULL);
  HRESULT_HANDLE(hr, ERROR, "CreateInputLayout failed");
  return true;
}

bool D3DVertexLayout::Init(RenderSystem* rs, ID3DBlob* blob) {
  HRESULT hr;
  DCHECK(typeid(*rs) == typeid(D3DRenderSystem));
  D3DRenderSystem* render_system = static_cast<D3DRenderSystem*>(rs);
  ID3D11Device* d3d_device = render_system->GetDevice();
  D3D11_INPUT_ELEMENT_DESC d3ddesc[kMaxInputElementDesc] = {0};
  CreateInputDesc(desc_, d3ddesc);
  hr = d3d_device->CreateInputLayout(d3ddesc,
                                     desc_->element_count(),
                                     (blob ? blob->GetBufferPointer() : NULL),
                                     (blob ? blob->GetBufferSize() : NULL),
                                     &input_layout_);
  HRESULT_HANDLE(hr, ERROR, "CreateInputLayout failed");
  return true;
}

// class D3DVertexBuffer
D3DVertexBuffer::D3DVertexBuffer(const Options &opt, D3DRenderSystem* rs)
    : VertexBuffer(opt)
    , locked_(false)
    , buffer_(NULL)
    , render_system_(rs) {
}

D3DVertexBuffer::~D3DVertexBuffer() {
  SAFE_RELEASE(buffer_);
}
bool D3DVertexBuffer::Init(SlotVertexData* dataptr) {
  DCHECK(element_size_ == -1 && buffer_size_ == -1 && vertex_count_ == -1);
  ID3D11Device* d3d_device = render_system_->GetDevice();

  // attention
  // If the bind flag is D3D11_BIND_CONSTANT_BUFFER, you must set the
  // ByteWidth value in multiples of 16, and less than or equal
  // to D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT.

  D3D11_BUFFER_DESC vb_desc;
  ZeroMemory(&vb_desc, sizeof(vb_desc));
  vb_desc.Usage = TranslateUsage(options_.usage);
  vb_desc.BindFlags = TranslateBindTarget(options_.target);
  vb_desc.CPUAccessFlags = TranslateCPUAccess(options_.cpu_access);
  vb_desc.MiscFlags = 0;
  vb_desc.ByteWidth = dataptr->buffer_size();

  D3D11_SUBRESOURCE_DATA d3d_vdata;
  ZeroMemory(&d3d_vdata, sizeof(d3d_vdata));
  d3d_vdata.pSysMem = dataptr->pointer();
  HRESULT hr;
  hr = d3d_device->CreateBuffer(&vb_desc, &d3d_vdata, &buffer_);
  HRESULT_HANDLE(hr, ERROR, "D3D11: CreateVertexBuffer failed ");

  element_size_ = dataptr->stride();
  buffer_size_ = dataptr->buffer_size();
  vertex_count_ = dataptr->vertex_count();

  layout_ = new D3DVertexLayout(dataptr->vertex_desc());
  CHECK(layout_->Init(render_system_));
  return true;
}

HardwareBufferDataPtr D3DVertexBuffer::map(MapType flags) {
  DCHECK(options_.usage & GraphicBuffer::kDynamic
         || options_.usage & GraphicBuffer::kStaging);
  DCHECK(options_.cpu_access & kCPUWrite || options_.cpu_access & kCPURead);

  HRESULT hr;
  DCHECK(!locked_) << "Vertex Buffer(" << options_.name << ") has been locked";
  ID3D11Device* d3d_device = render_system_->GetDevice();
  ID3D11DeviceContext* d3d_context = NULL;
  d3d_device->GetImmediateContext(&d3d_context);
  ScopedRefCOM autocom(d3d_context);
  D3D11_MAPPED_SUBRESOURCE mapped;
  D3D11_MAP d3d11_map = TranslateMapType(flags);
  hr = d3d_context->Map(buffer_, 0, d3d11_map, 0, &mapped);
  if (FAILED(hr)) {
    LOG(ERROR) << "D3D11 VertexBuffer: Map failed, desc: " << HRMessage(hr);
    return NULL;
  }

  HardwareBufferDataPtr data(new HardwareBufferData);
  SetLockDataPtr(mapped.pData, data.get());
  SetLockDataRowSize(mapped.RowPitch, data.get());
  SetLockDataColumnNum(mapped.DepthPitch, data.get());
  locked_ = true;
  return data;
}

void D3DVertexBuffer::unmap() {
  DCHECK(locked_) << "Vertex Buffer(" << options_.name << ") has not been locked";
  ID3D11Device* d3d_device = render_system_->GetDevice();
  ID3D11DeviceContext* d3d_context = NULL;
  d3d_device->GetImmediateContext(&d3d_context);
  ScopedRefCOM autocom(d3d_context);
  d3d_context->Unmap(buffer_, 0);
  locked_ = false;
}

// class D3DVertexBufferGroup
D3DVertexBufferGroup::D3DVertexBufferGroup(VertexDesc* desc, D3DRenderSystem* rs)
    : VertexBufferGroup(desc) {
  memset(vbs_, 0, sizeof(vbs_));
  memset(stride_, 0, sizeof(stride_));
  memset(offset_, 0, sizeof(offset_));
  layout_ = new D3DVertexLayout(desc);
  CHECK(layout_->Init(rs));
}

void D3DVertexBufferGroup::OnVertexBufferChanged() {
  GenVertexArray();
}

int32 D3DVertexBufferGroup::GenVertexArray() {
  for (int i = 0; i < vertex_buffer_count(); ++i) {
    D3DVertexBuffer* vb = (D3DVertexBuffer*)(vertex_buffer_at(i));
    DCHECK(vb->Initialized()) << "VertexBuffer not initialized.";
    vbs_[i] = vb->buffer_;
    stride_[i] = vb->element_size();
    offset_[i] = 0;
  }
  return vertex_buffer_count();
}
}  // namespace d3d11
}  // namespace azer
