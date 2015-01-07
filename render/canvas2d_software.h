#pragma once

#include "base/basictypes.h"
#include "azer/render/canvas2d.h"
#include "azer/render/texture.h"

class SkBitmap;
class SkCanvas;

namespace azer {

class Context2D;

class SoftwareCanvas2D : public Canvas2D {
 public:
  SoftwareCanvas2D(int width, int height, Context2D* context);

  ~SoftwareCanvas2D() override;
  bool Init() override;
  uint32 GetTexID() override;

  virtual SkCanvas* BeginPaint() override;
  virtual void EndPaint() override;
 private:
  bool InitTexture();
  bool UpdateTexture();
  std::unique_ptr<SkBitmap> skbitmap_;
  DISALLOW_COPY_AND_ASSIGN(SoftwareCanvas2D);
};
}  // namespace azer
