#include "azer/ui/compositor/bitmap_layer.h"

namespace azer {
namespace compositor {
BitmapLayer::BitmapLayer(LayerDelegate* delegate)
    : Layer(delegate) {
}

BitmapLayer::~BitmapLayer() {
}

void BitmapLayer::Render(Renderer* renderer, const gfx::Rect& parent_rc) {
}
}  // namespace compositor
}  // namespace azer
