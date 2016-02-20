#include "azer/render/layers/renderer_layer.h"

#include "third_party/skia/include/core/SkColor.h"
#include "azer/render/render.h"
#include "base/logging.h"

namespace azer {
namespace layers {

RendererLayer::RendererLayer()
    : TextureLayer(kRendererLayer) {
}

RendererLayer::~RendererLayer() {
}

void RendererLayer::SetBounds(const gfx::Rect& bounds) {
  SetBoundsInternal(bounds);
}

void RendererLayer::OnBoundsChanged() {
  if (layer_tree_host()) {
  }

  TextureLayer::OnBoundsChanged();
}

void RendererLayer::Redraw() {
  renderer_->Clear(Vector4(SkColorGetR(color()), 
                           SkColorGetG(color()), 
                           SkColorGetB(color()), 
                           SkColorGetA(color())));
}
}  // namespace layers
}  // namespace azer
