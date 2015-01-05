#pragma once

#include "azer/render/surface.h"
#include "azer/render/swap_chain.h"

namespace azer {
namespace d3d11 {

class D3DRenderSystem;
class D3DEnvironment;

class AngleSwapChain : public SwapChain {
 public:
  AngleSwapChain(D3DRenderSystem* rs);
  virtual ~AngleSwapChain();

  bool Init(Surface* surface);
  virtual bool reset(Surface* surface) override;
  virtual bool resize(Surface* surface) override;
  virtual bool Present() override;
 private:
  Renderer* CreateSurfaceRenderer(Surface* surface);
  D3DRenderSystem* render_system_;
  D3DEnvironment* envptr_;

  friend class D3DEnvironment;;
  DISALLOW_COPY_AND_ASSIGN(AngleSwapChain);
};
}  // namespace d3d11
}  // namespace azer
