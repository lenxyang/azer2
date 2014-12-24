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

Renderer* D3D11SwapChain::CreateDefaultRenderTarget(Surface* surface) {
  int32 width = surface->GetBounds().width();
  int32 height = surface->GetBounds().height();
  ID3D11DeviceContext* d3d_context = render_system_->GetContext();
  azer::Texture::Options o;
  o.width = width;
  o.height = height;
  o.target = Texture::kRenderTarget;
  std::unique_ptr<D3D11Renderer> renderer(
      new D3D11Renderer(d3d_context, render_system_));
  
  std::unique_ptr<D3D11RenderTarget> target(
          new D3D11RenderTarget(o, true, renderer.get()));
  if (!target->InitDefault(o, render_system_)) {
    return false;
  }

  Texture::Options depth_opt;
  depth_opt.width = o.width;
  depth_opt.height = o.height;
  depth_opt.format = kDepth24Stencil8;
  depth_opt.target = Texture::kDepthStencil;
  std::unique_ptr<D3D11DepthBuffer> depth(new D3D11DepthBuffer(depth_opt,
                                                               renderer.get()));
  if (!depth->Init(render_system_)) {
    return false;
  }

  if (!renderer->InitDefault(o, target.get(), depth.get())) {
    return false;
  }

  target.release();
  depth.release();
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

  renderer_.reset(CreateDefaultRenderTarget(surface));
  if (renderer_.get() == NULL) {
    return false;
  }

  return true;
}

}  // namespace azer
