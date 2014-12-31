#pragma once

#include "azer/render/canvas2d.h"
#include "azer/render/texture.h"

namespace azer {
namespace d3d11 {
class D3DCanvas2D : public Canvas2D {
 public:
  virtual TexturePtr InitTexture() override;
  virtual bool Init() override {
    return Canvas2D::Init();
  }
};
}  // namespace d3d11
}  // namespace azer
