#pragma once

#include <d3d11.h>

#include "base/basictypes.h"
#include "azer/render/blending.h"
#include "azer/render_system/d3d11/util.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;
class D3DRenderer;

class D3DBlending : public Blending {
 public:
  D3DBlending(const Desc& desc, D3DRenderSystem* rs)
        : Blending(desc)
        , blending_state_(NULL)
        , render_system_(rs) {
  }

  virtual ~D3DBlending() {
    SAFE_RELEASE(blending_state_);
  }

  bool Init();
 protected:
  ID3D11BlendState* blending_state_;
  D3DRenderSystem* render_system_;

  friend class D3DRenderer;
  DISALLOW_COPY_AND_ASSIGN(D3DBlending);
};
}  // namespace d3d11
}  // namespace azer
