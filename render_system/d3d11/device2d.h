#pragma once

#include <memory>

#include "base/basictypes.h"
#include "azer/base/export.h"

class GrTexture;
class SkGpuDevice;
class SkCanvas;

namespace azer {

class Context2D;
class Canvas2D;

class AZER_EXPORT D3DDevice2D {
 public:
  D3DDevice2D(){}
  ~D3DDevice2D() {}

  bool Init(Context2D* ctx, Canvas2D* canvas);

  SkCanvas* GetCanvas() { return sk_canvas_.get();}
  SkGpuDevice* GetDevice() { return gr_device_.get();}
  GrTexture* GetGrTex() { return grtex_;}
 private:
  std::unique_ptr<SkGpuDevice> gr_device_;
  std::unique_ptr<SkCanvas> sk_canvas_;
  GrTexture* grtex_;
  DISALLOW_COPY_AND_ASSIGN(D3DDevice2D);
};

}  // namespace azer
