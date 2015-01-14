#include "azer/ui/compositor/canvas_layer.h"


namespace azer {
namespace compositor {

CanvasLayer::CanvasLayer(LayerDelegate* delegate)
    : TextureLayer(delegate) {
}

CanvasLayer::~CanvasLayer() {
}

void CanvasLayer::Render(Renderer* renderer, const gfx::RectF& rect) {
}

}  // namespace compositor
}  // namespace azer
