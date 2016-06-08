#pragma once

#include "azer/render/fence.h"
#include "azer/render/renderer.h"
#include "azer/render_system/d3d11/d3d11.h"

namespace azer {
namespace d3d11 {

class D3DRenderer;

class D3DFence : public Fence {
 public:
  explicit D3DFence(D3DRenderer* renderer)
      : renderer_(renderer) 
      , query_(NULL) {
  }
  virtual ~D3DFence() {
    SAFE_RELEASE(query_);
  }

  virtual bool Begin(bool current) override;
  virtual bool End() override;

  bool Init();
 protected:
  ID3D11Query* query_;
  D3DRenderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(D3DFence);
};
}  // namespace d3d11
}  // namespace azer
