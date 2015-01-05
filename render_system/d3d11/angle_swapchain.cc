#include "azer/render_system/d3d11/angle_swapchain.h"

#include "EGL/egl.h"

namespace azer {
namespace d3d11 {

AngleSwapChain::AngleSwapChain(D3DRenderSystem* rs) {
}

AngleSwapChain::~AngleSwapChain() {
}

bool AngleSwapChain::Init(Surface* surface) {
  return true;
}

bool AngleSwapChain::reset(Surface* surface) {
  return true;
}

bool AngleSwapChain::resize(Surface* surface) {
  return true;
}

bool AngleSwapChain::Present() {
  return true;
}

Renderer* AngleSwapChain::CreateSurfaceRenderer(Surface* surface) {
  return NULL;
}
}  // namespace d3d11
}  // namespace azer
