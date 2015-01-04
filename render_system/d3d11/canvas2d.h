#pragma once

#include "base/basictypes.h"
#include "azer/render/canvas2d.h"
#include "azer/render/texture.h"


#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/gpu/GrTexture.h"
#include "third_party/skia/include/gpu/GrContext.h"
#include "third_party/skia/src/gpu/SkGpuDevice.h"


namespace azer {

class Context2D;

namespace d3d11 {

class D3DDevice2D;

class D3DCanvas2D : public Canvas2D {
 public:
  D3DCanvas2D(int width, int height, Context2D* context)
      : Canvas2D(width, height, context) {
  }

  ~D3DCanvas2D() override;
  bool Init() override;

 private:
  bool InitCanvas();
  bool InitTexture();
  std::unique_ptr<SkGpuDevice> gr_device_;
  std::unique_ptr<GrTexture> grtex_;
  D3DDevice2D* device_;
  DISALLOW_COPY_AND_ASSIGN(D3DCanvas2D);
};
}  // namespace d3d11
}  // namespace azer
