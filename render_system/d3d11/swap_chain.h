#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

#include <string>

#include "base/logging.h"
#include "azer/render/swap_chain.h"
#include "azer/render/surface.h"
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

  IDXGISwapChain* GetSwapChain();
 private:
  Renderer* CreateSurfaceRenderer(Surface* surface);
  D3DRenderSystem* render_system_;
  D3DEnvironmentPtr envptr_;
  
  DISALLOW_COPY_AND_ASSIGN(D3DSwapChain);
};

inline IDXGISwapChain* D3DSwapChain::GetSwapChain() {
  DCHECK(NULL != envptr_.get());
  return envptr_->GetSwapChain();
}
}  // namespace d3d11
}  // namespace azer
