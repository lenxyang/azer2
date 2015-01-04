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
  // order of GrTexture and SkGpuDevice is important
  // SkGpuDevice is dependent on GrTexture, so put it later
  // or will coredump
  std::unique_ptr<GrTexture> grtex_;
  std::unique_ptr<SkGpuDevice> gr_device_;
  DISALLOW_COPY_AND_ASSIGN(D3DCanvas2D);
};
}  // namespace d3d11
}  // namespace azer
