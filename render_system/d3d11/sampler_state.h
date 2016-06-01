#pragma once

#include "base/logging.h"
#include "azer/render/texture.h"
#include "azer/render_system/d3d11/d3d11.h"


namespace azer {
namespace d3d11 {
class D3DRenderer;
class D3DRenderSystem;

class D3DSamplerState: public SamplerState {
 public:
  D3DSamplerState(const Options& opt, D3DRenderSystem* rs);
  ~D3DSamplerState() override;
  ID3D11SamplerState* GetD3DSamplerState() { return sampler_state_;}
  bool Init() override;
 private:
  ID3D11SamplerState* sampler_state_;
  D3DRenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(D3DSamplerState);
};
}  // namespace d3d11
}  // namespace azer
