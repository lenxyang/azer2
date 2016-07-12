#include "azer/render_system/d3d11/structured_buffer.h"

#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_res_lock_helper.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {
// class D3DStructuredGpuBuffer
D3DStructuredGpuBuffer::D3DStructuredGpuBuffer(const GpuResOptions& options, 
                                               int count, int strip)
    : StructuredGpuBuffer(options, count, strip),
      bufobj_(NULL) {
}

D3DStructuredGpuBuffer::~D3DStructuredGpuBuffer() {
  SAFE_RELEASE(bufobj_);
}

GpuResLockDataPtr D3DStructuredGpuBuffer::map(MapType flags) {
  map_helper_.reset(new GpuResLockHelper(buffer_options(), bufobj_));
  return map_helper_->map(flags);
}

void D3DStructuredGpuBuffer::unmap() {
  CHECK(map_helper_.get());
  map_helper_->unmap();
  map_helper_.reset();
}

bool D3DStructuredGpuBuffer::Init(D3DRenderSystem* rs) {
  HRESULT hr;
  ID3D11Device* d3ddevice = rs->GetDevice();
  D3D11_BUFFER_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.BindFlags = TranslateBindTarget(buffer_options().target);
  desc.StructureByteStride = strip();
  desc.ByteWidth = this->size();
  desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
  hr = d3ddevice->CreateBuffer(&desc, NULL, &bufobj_);
  return true;
}


// class D3DUAStructuredGpuBufferView
D3DStructuredBufferResView::D3DStructuredBufferResView(GpuBuffer* buffer)
    : ShaderResView(ResViewType::kStructuredBuffer, buffer),
      res_view_(NULL) {
}

D3DStructuredBufferResView::~D3DStructuredBufferResView() {
  SAFE_RELEASE(res_view_);
}

bool D3DStructuredBufferResView::Init(D3DRenderSystem* rs) {
  HRESULT hr;
  D3D11_SHADER_RESOURCE_VIEW_DESC desc = {
    DXGI_FORMAT_UNKNOWN,
    D3D11_SRV_DIMENSION_BUFFER,
    0,
    0,
  };

  D3DStructuredGpuBuffer* buf = (D3DStructuredGpuBuffer*)buffer();
  ZeroMemory(&desc, sizeof(desc));
  // desc.Format = DXGI_FORMAT_UNKNOWN;
  //  desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
  desc.Buffer.NumElements = buf->count();
  ID3D11Device* d3ddevice = rs->GetDevice();
  hr = d3ddevice->CreateShaderResourceView(buf->object(), &desc, &res_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateShaderResourceView failed");
  return true;
}

// class D3DUAStructuredGpuBufferView
D3DUAStructuredBufferResView::D3DUAStructuredBufferResView(GpuBuffer* buffer)
    : UnorderAccessResView(ResViewType::kUAStructuredBuffer, buffer),
      unorder_view_(NULL) {
}

D3DUAStructuredBufferResView::~D3DUAStructuredBufferResView() {
  SAFE_RELEASE(unorder_view_);
}

bool D3DUAStructuredBufferResView::Init(D3DRenderSystem* rs) {
  HRESULT hr;
  D3DStructuredGpuBuffer* buf = (D3DStructuredGpuBuffer*)(this->buffer());
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
