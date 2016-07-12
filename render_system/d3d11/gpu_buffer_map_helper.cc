#include "azer/render_system/d3d11/gpu_buffer_map_helper.h"

#include "base/logging.h"
#include "azer/render/gpu_buffer.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {

GpuBufferMapHelper::GpuBufferMapHelper(const GpuBufferOptions& options,
                                       ID3D11Buffer* buffer) 
    : options_(options),
      buffer_(buffer),
      locked_(false)  {
  buffer->AddRef();
}

GpuBufferMapHelper::~GpuBufferMapHelper() {
  CHECK(locked_) << "lock not freed";
  SAFE_RELEASE(buffer_);
}

GpuBufferLockDataPtr GpuBufferMapHelper::map(MapType flags) {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  DCHECK(options_.usage & kBufferDynamic
         || options_.usage & kBufferStaging);
  DCHECK(options_.cpu_access & kCPUWrite || options_.cpu_access & kCPURead);

  HRESULT hr;
  DCHECK(!locked_) << "Vertex Buffer(" << options_.name << ") has been locked";
  ID3D11Device* d3d_device = rs->GetDevice();
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

  GpuBufferLockDataPtr data(new GpuBufferLockData((uint8_t*)mapped.pData,
                                                  mapped.RowPitch,
                                                  mapped.DepthPitch));
  locked_ = true;
  return data;
}

void GpuBufferMapHelper::unmap() {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  DCHECK(locked_) << "Vertex Buffer(" << options_.name << ") has not been locked";
  ID3D11Device* d3d_device = rs->GetDevice();
  ID3D11DeviceContext* d3d_context = NULL;
  d3d_device->GetImmediateContext(&d3d_context);
  ScopedRefCOM autocom(d3d_context);
  d3d_context->Unmap(buffer_, 0);
  locked_ = false;
}


// class GpuTexLockHelper
GpuTexLockHelper::GpuTexLockHelper(const GpuBufferOptions& options,
                                   ID3D11Resource* buffer) 
    : options_(options),
      buffer_(buffer),
      locked_(false)  {
  buffer->AddRef();
}

GpuTexLockHelper::~GpuTexLockHelper() {
  CHECK(locked_) << "lock not freed";
  SAFE_RELEASE(buffer_);
}

GpuBufferLockDataPtr GpuTexLockHelper::map(MapType flags) {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  DCHECK(options_.usage & kBufferDynamic
         || options_.usage & kBufferStaging);
  DCHECK(options_.cpu_access & kCPUWrite || options_.cpu_access & kCPURead);

  HRESULT hr;
  DCHECK(!locked_) << "Vertex Buffer(" << options_.name << ") has been locked";
  ID3D11Device* d3d_device = rs->GetDevice();
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

  GpuBufferLockDataPtr data(new GpuBufferLockData((uint8_t*)mapped.pData,
                                                  mapped.RowPitch,
                                                  mapped.DepthPitch));
  locked_ = true;
  return data;
}

void GpuTexLockHelper::unmap() {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  DCHECK(locked_) << "Vertex Buffer(" << options_.name << ") has not been locked";
  ID3D11Device* d3d_device = rs->GetDevice();
  ID3D11DeviceContext* d3d_context = NULL;
  d3d_device->GetImmediateContext(&d3d_context);
  ScopedRefCOM autocom(d3d_context);
  d3d_context->Unmap(buffer_, 0);
  locked_ = false;
}
}  // namespace d3d11
}  // namespace azer
