#include "azer/render_system/d3d11/gpu_res_lock_helper.h"

#include "base/logging.h"
#include "base/bind.h"
#include "base/callback.h"
#include "azer/render/gpu_resource.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {

// class GpuResLockHelper
GpuResLockHelper::GpuResLockHelper(const GpuResOptions& options,
                                   ID3D11Resource* buffer) 
    : options_(options),
      buffer_(buffer),
      locked_(false)  {
}

GpuResLockHelper::~GpuResLockHelper() {
  CHECK(!locked_) << "lock not freed";
}

GpuResLockDataPtr GpuResLockHelper::map(MapType flags) {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  DCHECK(options_.usage & kBufferDynamic
         || options_.usage & kBufferStaging);
  DCHECK(options_.cpu_access & kCPUWrite || options_.cpu_access & kCPURead);

  HRESULT hr;
  ID3D11Device* d3d_device = rs->GetDevice();
  ID3D11DeviceContext* d3d_context = NULL;
  d3d_device->GetImmediateContext(&d3d_context);
  ScopedRefCOM autocom(d3d_context);
  D3D11_MAPPED_SUBRESOURCE mapped = { 0 };
  D3D11_MAP d3d11_map = TranslateMapType(flags);
  hr = d3d_context->Map(buffer_, 0, d3d11_map, 0, &mapped);
  if (FAILED(hr)) {
    LOG(ERROR) << "D3D11 GpuResLock: Map failed, desc: " << HRMessage(hr);
    return NULL;
  }

  ::base::Callback<void()> callback = ::base::Bind(&GpuResLockHelper::unmap, this);
  GpuResLockDataPtr data(new GpuResLockData((uint8_t*)mapped.pData,
                                            mapped.RowPitch,
                                            mapped.DepthPitch,
                                            callback));
  locked_ = true;
  return data;
}

void GpuResLockHelper::unmap() {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11Device* d3d_device = rs->GetDevice();
  ID3D11DeviceContext* d3d_context = NULL;
  d3d_device->GetImmediateContext(&d3d_context);
  ScopedRefCOM autocom(d3d_context);
  d3d_context->Unmap(buffer_, 0);
  locked_ = false;
}
}  // namespace d3d11
}  // namespace azer
