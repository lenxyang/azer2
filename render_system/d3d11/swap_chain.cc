#include "azer/render_system/d3d11/swap_chain.h"

#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/render_target.h"
#include "azer/render_system/d3d11/depth_buffer.h"
#include "azer/render_system/d3d11/dx3d_util.h"
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

RendererPtr D3DSwapChain::CreateSurfaceRenderer(Surface* surface) {
  Texture::Options opt;
  opt.format = kTexRGBA8UNorm;
  opt.size = gfx::Size(surface->GetBounds().size());
  opt.target = (kBindTargetRenderTarget);
  opt.sample_desc.count = surface->sample_desc().count;
  opt.sample_desc.quality = surface->sample_desc().quality;
  scoped_refptr<D3DTexture2D> ptr(new D3DTexture2D(opt, render_system_));
  ID3D11Texture2D* buffer = GetD3DEnvSwapChain()->GetSwapTexture();
  ptr->Attach(buffer);
  RenderTargetPtr rt = render_system_->CreateRenderTarget(
      RenderTarget::Options(), ptr.get());
 
  opt.format = kTexR24G8;
  opt.target = kBindTargetDepthStencil | kBindTargetShaderResource;
  scoped_refptr<D3DTexture2D> tex = new D3DTexture2D(opt, render_system_);
  if (!tex->Init(NULL, 1, 1)) {
    return RendererPtr();
  }

  DepthBufferPtr depth = render_system_->CreateDepthBuffer(
      DepthBuffer::Options(), tex.get());
  if (!depth.get() || !rt.get()) {
    return RendererPtr();
  }

  RenderTargetPtrs targets;
  targets.push_back(rt);
  RendererPtr renderer = render_system_->CreateRenderer(&targets, depth.get());
  return renderer;
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
  renderer_ = CreateSurfaceRenderer(surface);
  if (renderer_.get() == NULL) {
    return false;
  }

  return true;
}

}  // namespace d3d11
}  // namespace azer

