#include "azer/ui/compositor/bitmap_layer.h"

namespace azer {
namespace compositor {
BitmapLayer::BitmapLayer()
    : Layer(kBitmapLayer) {
}

BitmapLayer::~BitmapLayer() {
}

void BitmapLayer::Render(Renderer* renderer, const gfx::Rect& parent_rc) {
}

void BitmapLayer::SetBounds(const gfx::Rect& bounds) {
}
}  // namespace compositor
}  // namespace azer
