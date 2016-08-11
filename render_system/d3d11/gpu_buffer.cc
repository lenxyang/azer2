#include "azer/render_system/d3d11/gpu_buffer.h"

#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_res_lock_helper.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {
// class D3DGpuBuffer
D3DGpuBuffer::D3DGpuBuffer(const GpuResOptions& options, 
                                               int count, int strip)
    : GpuBuffer(options, count, strip),
      bufobj_(NULL) {
}

D3DGpuBuffer::~D3DGpuBuffer() {
  SAFE_RELEASE(bufobj_);
}

void D3DGpuBuffer::UpdateData(const uint8_t* data, int64_t size) {
  DCHECK(bufobj_) << "StructedGpuBuffer Not Initialized";
  DCHECK_EQ(size, this->size());
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11DeviceContext* d3d_context = rs->GetContext();
  d3d_context->UpdateSubresource(bufobj_, 0, NULL, data, 0, 0);
}

GpuResLockDataPtr D3DGpuBuffer::map(MapType flags) {
  map_helper_.reset(new GpuResLockHelper(resource_options(), bufobj_));
  return map_helper_->map(flags);
}

void D3DGpuBuffer::unmap() {
  CHECK(map_helper_.get());
  map_helper_->unmap();
  map_helper_.reset();
}

bool D3DGpuBuffer::CopyTo(GpuResource* res) {
  D3DGpuBuffer* target = (D3DGpuBuffer*)res;
  if(target->size() != this->size()
     || target->strip() != this->strip()
     || target->count() != this->count()) {
    return false;
  }

  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11DeviceContext* d3d_context = rs->GetContext();
  d3d_context->CopyResource(target->object(), this->object());
  return true;
}

bool D3DGpuBuffer::Init(D3DRenderSystem* rs, const uint8_t* data) {
  HRESULT hr;
  ID3D11Device* d3ddevice = rs->GetDevice();
  D3D11_BUFFER_DESC desc;
  GenBufferDesc(resource_options(), &desc);
  desc.ByteWidth = this->size();
  if ((uint32_t)resource_type() & (uint32_t)GpuResType::kStructuredBuffer) {
    desc.StructureByteStride = strip();
    desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
  }

  if (data) {
    D3D11_SUBRESOURCE_DATA subdata;
    ZeroMemory(&subdata, sizeof(subdata));
    subdata.pSysMem = data;
    hr = d3ddevice->CreateBuffer(&desc, &subdata, &bufobj_);
    HRESULT_HANDLE(hr, ERROR, "CreateGpuBuffer failed");
  } else {
    hr = d3ddevice->CreateBuffer(&desc, NULL, &bufobj_);
    HRESULT_HANDLE(hr, ERROR, "CreateGpuBuffer failed");
  }

  return true;
}

void D3DGpuBuffer::SetName(const std::string& name) {
  DCHECK(bufobj_);
  bufobj_->SetPrivateData(WKPDID_D3DDebugObjectName,
                          (UINT)name.length(), name.c_str());
}

// class D3DUAGpuBufferView
D3DBufferResView::D3DBufferResView(GpuResource* buffer)
    : ShaderResView(ResViewType::kBufferResView, buffer),
      res_view_(NULL) {
}

D3DBufferResView::~D3DBufferResView() {
  SAFE_RELEASE(res_view_);
}

bool D3DBufferResView::Init(D3DRenderSystem* rs) {
  HRESULT hr;
  D3DGpuBuffer* buf = (D3DGpuBuffer*)buffer();
  D3D11_SHADER_RESOURCE_VIEW_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Format = DXGI_FORMAT_UNKNOWN;
  desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
  desc.Buffer.NumElements = buf->count();
  desc.Buffer.ElementWidth = buf->strip();
  ID3D11Device* d3ddevice = rs->GetDevice();
  hr = d3ddevice->CreateShaderResourceView(buf->object(), &desc, &res_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateShaderResourceView failed");
  return true;
}

// class D3DUAGpuBufferView
D3DBufferUAView::D3DBufferUAView(GpuResource* buffer)
    : UnorderAccessView(ResViewType::kBufferUAView, buffer),
      unorder_view_(NULL) {
}

D3DBufferUAView::~D3DBufferUAView() {
  SAFE_RELEASE(unorder_view_);
}

bool D3DBufferUAView::Init(D3DRenderSystem* rs) {
  HRESULT hr;
  D3DGpuBuffer* buf = (D3DGpuBuffer*)(this->buffer());
  D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Format = DXGI_FORMAT_UNKNOWN;
  desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
  desc.Buffer.NumElements = buf->count();
  ID3D11Device* d3ddevice = rs->GetDevice();
  hr = d3ddevice->CreateUnorderedAccessView(buf->object(), &desc, &unorder_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateShaderResourceView failed");
  return true;
}
}  // namespace d3d11
}  // namespace azer
