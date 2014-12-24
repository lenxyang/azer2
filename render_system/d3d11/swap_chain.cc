#include "azer/render_system/d3d11/swap_chain.h"

#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/render_target.h"
#include "azer/render_system/d3d11/depth_buffer.h"
#include "azer/render_system/d3d11/util.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/ui/window/window_host.h"

namespace azer {
D3D11SwapChain::D3D11SwapChain(D3D11RenderSystem* rs)
    : render_system_(rs)
    , envptr_(rs->GetD3DEnv()) {
}

D3D11SwapChain::~D3D11SwapChain() {
}

bool D3D11SwapChain::Init(Surface* surface)  {
  return reset(surface);
}

Renderer* D3D11SwapChain::CreateSurfaceRenderer(Surface* surface) {
  ID3D11DeviceContext* d3d_context = render_system_->GetContext();
  std::unique_ptr<D3D11SurfaceRenderer> renderer(
      new D3D11SurfaceRenderer(surface, d3d_context, render_system_));
  
  RenderTargetPtr target(D3D11SurfaceRenderTarget::Create(
      surface, renderer.get()));
  DepthBufferPtr depth(D3D11DepthBuffer::Create(surface, renderer.get()));
  if (!renderer->InitForSurface(target, depth)) {
    return false;
  }

  return renderer.release();
}

bool D3D11SwapChain::resize(Surface* surface) {
  CHECK(false);
  return true;
}

bool D3D11SwapChain::Present() {
  D3D11EnvironmentPtr& ptr = render_system_->GetD3DEnv();
  IDXGISwapChain* swap_chain = ptr->GetSwapChain();
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

bool D3D11SwapChain::reset(Surface* surface) {
  D3D11EnvironmentPtr& ptr = render_system_->GetD3DEnv();
  ptr->ResetSwapChain();

  renderer_.reset(CreateSurfaceRenderer(surface));
  if (renderer_.get() == NULL) {
    return false;
  }

  return true;
}

}  // namespace azer
