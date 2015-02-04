#include "azer/render/layers/bitmap_layer.h"

namespace azer {
namespace layers {
BitmapLayer::BitmapLayer()
    : Layer(kBitmapLayer) {
}

BitmapLayer::~BitmapLayer() {
}

void BitmapLayer::Render(Renderer* renderer, const gfx::Rect& parent_rc) {
}

void BitmapLayer::SetBounds(const gfx::Rect& bounds) {
}
}  // namespace layers
}  // namespace azer
