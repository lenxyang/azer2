#pragma once

#include "base/basictypes.h"
#include "azer/render/technique.h"
#include "azer/render/shader.h"

namespace azer {
namespace d3d11 {

class D3DRenderSystem;

class D3DTechnique : public Technique {
 public:
  D3DTechnique(D3DRenderSystem* rs)
      : render_system_(rs) {
  }

  TechniquePtr CopyTechnique() override;
  void Use(Renderer*) override;
 private:
  D3DRenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(D3DTechnique);
};
}  // namespace d3d11
}  // namespace azer
