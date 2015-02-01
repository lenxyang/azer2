#include "azer/render/compositor/renderer_layer.h"

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
  if (GetTreeHost()) {
  }

  SetBoundsInternal(bounds);
}

void RendererLayer::Redraw() {
  renderer_->Clear(Vector4(SkColorGetR(color()), 
                           SkColorGetG(color()), 
                           SkColorGetB(color()), 
                           SkColorGetA(color())));
}
}  // namespace compositor
}  // namespace azer
