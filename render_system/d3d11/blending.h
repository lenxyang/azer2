#pragma once

#include "azer/render/blending.h"
#include "azer/render_system/d3d11/d3d11.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;
class D3DRenderer;

class D3DBlending : public Blending {
 public:
  D3DBlending(const BlendDesc& desc, D3DRenderSystem* rs);
  ~D3DBlending() override;

  bool Init();
  bool OnDescChanged() override;
 protected:
  ID3D11BlendState* blending_state_;
  D3DRenderSystem* render_system_;

  friend class D3DRenderer;
  DISALLOW_COPY_AND_ASSIGN(D3DBlending);
};
}  // namespace d3d11
}  // namespace azer
