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
    : swap_chain_(NULL)
    , render_system_(rs) {
}

D3D11SwapChain::~D3D11SwapChain() {
  SAFE_RELEASE(swap_chain_);
}

bool D3D11SwapChain::Init(int width, int height)  {
  DCHECK(NULL == swap_chain_);
  return reset(width, height);
}

Renderer* D3D11SwapChain::CreateDefault(int width, int height) {
  ID3D11DeviceContext* d3d_context = render_system_->GetContext();
  azer::Texture::Options o;
  o.width = width;
  o.height = height;
  o.target = Texture::kRenderTarget;
  std::unique_ptr<D3D11Renderer> renderer(
      new D3D11Renderer(d3d_context, render_system_));
  
  std::unique_ptr<D3D11RenderTarget> target(
          new D3D11RenderTarget(o, true, renderer.get()));
  if (!target->InitDefault(o, this, render_system_)) {
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

bool D3D11SwapChain::resize(int width, int height) {
  CHECK(false);
  return true;
}

bool D3D11SwapChain::Present() {
  DCHECK(NULL != swap_chain_);
  HRESULT hr = swap_chain_->Present(0, 0);
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

bool D3D11SwapChain::reset(int width, int height) {
  HRESULT_HANDLE(hr, ERROR, "Failed to create D3D11 and Swapchain ");
  renderer_.reset(CreateDefault(width, height));
  if (renderer_.get() == NULL) {
    return false;
  }

  return true;
}

}  // namespace azer
