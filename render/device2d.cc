#include "azer/render/device2d.h"

#include "SkCanvas.h"
#include "SkGpuDevice.h"
#include "GrContext.h"
#include "GrTexture.h"

#include "azer/render/texture.h"
#include "azer/render/context2d.h"
#include "azer/render/canvas2d.h"

namespace azer {

bool Device2D::Init(Context2D* ctx, Canvas2D* canvas) {
  GrContext* context = ctx->gr_context_;
  grtex_ = ctx->CreateTexture(canvas->width(), canvas->height());
  // tex_.reset(ctx->GetAzerEGLInterface()->CreateTexture(gltex_->fbid()));
  SkSurfaceProps props(SkSurfaceProps::kDisallowAntiAlias_Flag,
                       kRGB_H_SkPixelGeometry);
  gr_device_.reset(SkGpuDevice::Create(grtex_, props, 0));
  if (gr_device_.get() == NULL) {
    LOG(ERROR) << "Failed to create SkGpuDevice";
    return false;
  }

  sk_canvas_.reset(new SkCanvas(gr_device_.get()));
  if (sk_canvas_.get() == NULL) {
    LOG(ERROR) << "Failed to create SkCanvas";
    return false;
  }

  return true;
}
}  // namespace azer
