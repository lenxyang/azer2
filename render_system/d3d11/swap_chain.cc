#include "azer/render_system/d3d11/swap_chain.h"

#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/render_target.h"
#include "azer/render_system/d3d11/depth_buffer.h"
#include "azer/render_system/d3d11/util.h"
#include "azer/render_system/d3d11/texture.h"

namespace azer {
namespace d3d11 {
D3DSwapChain::D3DSwapChain(D3DRenderSystem* rs)
    : render_system_(rs) {
}

D3DSwapChain::~D3DSwapChain() {
}

bool D3DSwapChain::Init(Surface* surface)  {
  return reset(surface);
}

Renderer* D3DSwapChain::CreateSurfaceRenderer(Surface* surface) {
  ID3D11DeviceContext* d3d_context = render_system_->GetContext();
  std::unique_ptr<D3DSurfaceRenderer> renderer(
      new D3DSurfaceRenderer(surface, d3d_context, render_system_));
  
  RenderTargetPtr target(D3DSurfaceRenderTarget::Create(d3d_swapchain_.get(),
                                                        render_system_));
  DepthBufferPtr depth(D3DDepthBuffer::Create(surface, render_system_));
  if (!renderer->InitForSurface(target, depth)) {
    return false;
  }

  return renderer.release();
}

bool D3DSwapChain::resize(Surface* surface) {
  CHECK(false);
  return true;
}

bool D3DSwapChain::Present() {
  DCHECK(d3d_swapchain_.get());
  IDXGISwapChain* swap_chain = d3d_swapchain_->GetSwapChain();
  DCHECK(NULL != swap_chain);
  HRESULT hr = swap_chain->Present(0, 0);
  if (FAILED(hr)) {
    if (hr == DXGI_ERROR_DEVICE_RESET) {
      LOG(ERROR) << "Present error: DXGI_ERROR_DEVICE_RESET";
    } else if (hr == DXGI_ERROR_DEVICE_REMOVED) {
      LOG(ERROR) << "Present error: DXGI_ERROR_DEVICE_REMOVED";
    } else if (hr == DXGI_STATUS_OCCLUDED) {
      LOG(ERROR) << "Present error: DXGI_STATUS_OCCLUDED";
    } else {
      CHECK(false) << "unknown";
    }
    return false;
  }

  return true;
}

bool D3DSwapChain::reset(Surface* surface) {
  D3DEnvironmentPtr ptr = render_system_->GetD3DEnv();
  d3d_swapchain_.reset(ptr->CreateSwapChain(surface));
  renderer_.reset(CreateSurfaceRenderer(surface));
  if (renderer_.get() == NULL) {
    return false;
  }

  return true;
}

}  // namespace d3d11
}  // namespace azer

