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
class D3D11RenderSystem;
class D3D11Renderer;

class D3D11SwapChain : public SwapChain {
 public:
  D3D11SwapChain(D3D11RenderSystem* rs);
  virtual ~D3D11SwapChain();

  bool Init(Surface* surface);
  virtual bool reset(Surface* surface) override;
  virtual bool resize(Surface* surface) override;
  virtual bool Present() override;

  IDXGISwapChain* GetSwapChain();
 private:
  Renderer* CreateDefaultRenderTarget(Surface* surface);
  D3D11RenderSystem* render_system_;
  D3D11EnvironmentPtr envptr_;
  
  DISALLOW_COPY_AND_ASSIGN(D3D11SwapChain);
};

inline IDXGISwapChain* D3D11SwapChain::GetSwapChain() {
  DCHECK(NULL != envptr_.get());
  return envptr_->GetSwapChain();
}
}  // namespace azer
