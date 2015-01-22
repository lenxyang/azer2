#include "azer/ui/compositor/renderer_layer.h"

#include "third_party/skia/include/core/SkColor.h"
#include "azer/render/render.h"
#include "base/logging.h"

namespace azer {
namespace compositor {

RendererLayer::RendererLayer()
    : TextureLayer(kRendererLayer) {
}

RendererLayer::~RendererLayer() {
}

void RendererLayer::SetBounds(const gfx::Rect& bounds) {
  SetBoundsInternal(bounds);
}

void RendererLayer::Redraw() {
  renderer_->Clear(Vector4(SkColorGetR(background_color_), 
                           SkColorGetG(background_color_), 
                           SkColorGetB(background_color_), 
                           SkColorGetA(background_color_)));
}
}  // namespace compositor
}  // namespace azer
