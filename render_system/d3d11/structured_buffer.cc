#include "azer/render_system/d3d11/structured_buffer.h"

#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_buffer_map_helper.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {
// class D3DStructuredGpuBuffer
D3DStructuredGpuBuffer::D3DStructuredGpuBuffer(const GpuBufferOptions& options, 
                                               int count, int strip)
    : StructuredGpuBuffer(options, count, strip),
      bufobj_(NULL) {
}

D3DStructuredGpuBuffer::~D3DStructuredGpuBuffer() {
  SAFE_RELEASE(bufobj_);
}

GpuBufferLockDataPtr D3DStructuredGpuBuffer::map(MapType flags) {
  map_helper_.reset(new GpuBufferMapHelper(buffer_options(), bufobj_));
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
D3DUAStructuredGpuBufferView::D3DUAStructuredGpuBufferView(
    const Options& opt, StructuredGpuBuffer* buffer)
    : StructuredGpuBufferView(opt, buffer),
      unorder_view_(NULL) {
}

D3DUAStructuredGpuBufferView::~D3DUAStructuredGpuBufferView() {
  SAFE_RELEASE(unorder_view_);
}

bool D3DUAStructuredGpuBufferView::Init(D3DRenderSystem* rs) {
  HRESULT hr;
  D3DStructuredGpuBuffer* buffer = (D3DStructuredGpuBuffer*)(this->buffer());
  D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Format = DXGI_FORMAT_UNKNOWN;
  desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
  desc.Buffer.NumElements = buffer->count();
  ID3D11Device* d3ddevice = rs->GetDevice();
  hr = d3ddevice->CreateUnorderedAccessView(buffer->object(), &desc, &unorder_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateShaderResourceView failed");
  return true;
}
}  // namespace d3d11
}  // namespace azer
