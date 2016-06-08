#pragma once

#include <string>

#include "base/logging.h"
#include "azer/render/swap_chain.h"
#include "azer/render/surface.h"
#include "azer/render_system/d3d11/d3d11.h"
#include "azer/render_system/d3d11/d3denv.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;
class D3DRenderer;

class D3DSwapChain : public SwapChain {
 public:
  D3DSwapChain(D3DRenderSystem* rs);
  virtual ~D3DSwapChain();

  bool Init(Surface* surface);
  virtual bool reset(Surface* surface) override;
  virtual bool resize(Surface* surface) override;
  virtual bool Present() override;

  D3DEnvSwapChain* GetD3DEnvSwapChain() { return d3d_swapchain_.get();}
 private:
  RendererPtr CreateSurfaceRenderer(Surface* surface);
  D3DRenderSystem* render_system_;
  std::unique_ptr<D3DEnvSwapChain> d3d_swapchain_;
  DISALLOW_COPY_AND_ASSIGN(D3DSwapChain);
};

}  // namespace d3d11
}  // namespace azer
