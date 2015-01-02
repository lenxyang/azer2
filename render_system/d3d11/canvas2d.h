#pragma once

#include "base/basictypes.h"
#include "azer/render/canvas2d.h"
#include "azer/render/texture.h"

namespace azer {

class Context2D;

namespace d3d11 {

class D3DDevice2D;

class D3DCanvas2D : public Canvas2D {
 public:
  D3DCanvas2D(int width, int height, Context2D* context)
      : Canvas2D(width, height, context) {
  }

  virtual TexturePtr InitTexture(int32 texid) override;
  virtual bool Init() override {
    return Canvas2D::Init();
  }
 private:
  D3DDevice2D* device_;
  DISALLOW_COPY_AND_ASSIGN(D3DCanvas2D);
};
}  // namespace d3d11
}  // namespace azer
