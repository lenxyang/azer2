#pragma once

#include "azer/render/context2d.h"

struct GrGLInterface;
class GrRenderTarget;
class GrContext;
class GrTexture;
class SkGpuDevice;
class SkCanvas;

namespace azer {

class Device2D {
 public:
  Device2D(){}
  ~Device2D() {}

  bool Init(Context2D* ctx, Canvas2D* canvas);

  SkCanvas* GetCanvas() { return sk_canvas_.get();}
  SkGpuDevice* GetDevice() { return gr_device_.get();}
  GrTexture* GetGrTex() { return grtex_;}
 private:
  GrTexture* CreateTex(Context2D* context, Canvas2D* canvas);
  std::unique_ptr<SkGpuDevice> gr_device_;
  std::unique_ptr<SkCanvas> sk_canvas_;
  GrTexture* grtex_;
  DISALLOW_COPY_AND_ASSIGN(Device2D);
};

}  // namespace azer
