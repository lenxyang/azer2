#include "azer/render_system/d3d11/vertex_buffer.h"

#include <windows.h>
#include "base/logging.h"
#include "azer/base/string.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {

bool D3DVertexBuffer::Init(const SlotVertexData* dataptr) {
  DCHECK(element_size_ == -1 && buffer_size_ == -1 && vertex_count_ == -1);
  ID3D11Device* d3d_device = render_system_->GetDevice();

  // attention
  // If the bind flag is D3D11_BIND_CONSTANT_BUFFER, you must set the
  // ByteWidth value in multiples of 16, and less than or equal
  // to D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT.

  D3D11_BUFFER_DESC vb_desc;
  ZeroMemory(&vb_desc, sizeof(vb_desc));
  vb_desc.Usage = TranslateUsage(options_.usage);
  vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
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
D3DVertexBufferGroup::D3DVertexBufferGroup(VertexDescPtr desc)
    : VertexBufferGroup(desc) {
  memset(vbs_, 0, sizeof(vbs_));
  memset(stride_, 0, sizeof(stride_));
  memset(offset_, 0, sizeof(offset_));
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
