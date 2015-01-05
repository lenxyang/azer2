#pragma once

#include <d3d11.h>

#include "azer/render/surface.h"
#include "azer/render/swap_chain.h"
#include "azer/render_system/d3d11/overlay.h"

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

  RendererPtr combined_renderer_;

  // combined the texture angle with swapchain default renderer
  // into combined_renderer_, and return combined_rederer_'s resource pointer
  friend void Combined(ID3D11ShaderResourceView* angle, AngleSwapChain* swapchain,
                       ID3D11ShaderResourceView** resource);
  friend class D3DEnvironment;
  DISALLOW_COPY_AND_ASSIGN(AngleSwapChain);
};
}  // namespace d3d11
}  // namespace azer
