#include "azer/ui/compositor/canvas_layer.h"


namespace azer {
namespace compositor {

CanvasLayer::CanvasLayer(LayerDelegate* delegate)
    : TextureLayer(delegate) {
}

CanvasLayer::~CanvasLayer() {
}

void CanvasLayer::Render(Renderer* renderer, const gfx::Rect& parent_rc) {
}

void CanvasLayer::SetBounds(const gfx::Rect& bounds) {
  SetBoundsInternal(bounds);
}
}  // namespace compositor
}  // namespace azer
