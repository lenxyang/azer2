#include "azer/render_system/d3d11/angle_swapchain.h"

#include "azer/render/texture.h"
#include "azer/render/surface.h"
#include "azer/render_system/d3d11/angle_env.h"
#include "azer/render_system/d3d11/angle_interface.h"
#include "azer/render_system/d3d11/d3denv.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/texture.h"

extern "C" {
void SwapRectHookANGLE(ID3D11ShaderResourceView* angle, 
                       ID3D11ShaderResourceView** resource);
}

namespace azer {
namespace d3d11 {

class TextureCombined {
 public:
 private:
  OverlayPtr overlay_;
  DISALLOW_COPY_AND_ASSIGN(TextureCombined);
};

void Combined(ID3D11ShaderResourceView* angle, AngleSwapChain* swapchain,
              ID3D11ShaderResourceView** resource) {
  RenderTargetPtr target = swapchain->GetRenderer()->GetRenderTarget();
  TexturePtr& tex = target->GetTexture();
  D3DTexture* d3dtex = (D3DTexture*)tex.get();
  *resource = d3dtex->GetResourceView();
}

AngleSwapChain::AngleSwapChain(D3DRenderSystem* rs)
    : render_system_(rs) {
}

AngleSwapChain::~AngleSwapChain() {
}

bool AngleSwapChain::Init(Surface* surface) {
  DCHECK(NULL != pfnSetSwapRectHook);
  (*pfnSetSwapRectHook)(SwapRectHookANGLE);
  return reset(surface);
}

bool AngleSwapChain::reset(Surface* surface) {
  D3DEnvironmentPtr& ptr = render_system_->GetD3DEnv();
  renderer_ = CreateSurfaceRenderer(surface);
  if (renderer_.get() == NULL) {
    return false;
  }

  combined_renderer_ = CreateSurfaceRenderer(surface);
  if (combined_renderer_.get() == NULL) {
    return false;
  }

  return true;
}

bool AngleSwapChain::resize(Surface* surface) {
  return reset(surface);
}

bool AngleSwapChain::Present() {
  return AngleEnv::Pointer()->swap();
}

Renderer* AngleSwapChain::CreateSurfaceRenderer(Surface* surface) {
  Texture::Options opt;
  opt.size = surface->GetBounds().size();
  opt.target = (azer::Texture::BindTarget)
      (azer::Texture::kRenderTarget | azer::Texture::kShaderResource);
  return render_system_->CreateRenderer(opt);
}
}  // namespace d3d11
}  // namespace azer

using azer::d3d11::AngleSwapChain;
extern "C" {
void SwapRectHookANGLE(ID3D11ShaderResourceView* angle, 
                       ID3D11ShaderResourceView** resource) {
  /*
  azer::RenderSystem* rs = azer::RenderSystem::Current();
  AngleSwapChain* swapchain = (AngleSwapChain*)(rs->GetSwapchain().get());
  Combined(angle, swapchain, resource);
  */
}
}
