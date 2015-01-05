#include "azer/render_system/d3d11/angle_swapchain.h"

#include "azer/render/texture.h"
#include "azer/render/surface.h"
#include "azer/render_system/d3d11/angle_env.h"
#include "azer/render_system/d3d11/angle_interface.h"
#include "azer/render_system/d3d11/d3denv.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {

AngleSwapChain::AngleSwapChain(D3DRenderSystem* rs) {
}

AngleSwapChain::~AngleSwapChain() {
}

bool AngleSwapChain::Init(Surface* surface) {
  return reset(surface);
}

bool AngleSwapChain::reset(Surface* surface) {
  D3DEnvironmentPtr& ptr = render_system_->GetD3DEnv();
  ptr->ResetSwapChain();

  renderer_.reset(CreateSurfaceRenderer(surface));
  if (renderer_.get() == NULL) {
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
  opt.width = surface->GetBounds().width();
  opt.height = surface->GetBounds().height();
  opt.target = (azer::Texture::BindTarget)
      (azer::Texture::kRenderTarget | azer::Texture::kShaderResource);
  return render_system_->CreateRenderer(opt);
}
}  // namespace d3d11
}  // namespace azer
