#pragma once


#include "azer/render/technique.h"
#include "azer/render/shader.h"

namespace azer {
namespace d3d11 {

class D3DRenderSystem;

class D3DTechnique : public Technique {
 public:
  D3DTechnique() {}
  TechniquePtr CopyTechnique() override;
 private:
  DISALLOW_COPY_AND_ASSIGN(D3DTechnique);
};
}  // namespace d3d11
}  // namespace azer
