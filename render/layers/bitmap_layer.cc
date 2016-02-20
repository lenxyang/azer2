#include "azer/render/layers/bitmap_layer.h"

#include "base/logging.h"

namespace azer {
namespace layers {
BitmapLayer::BitmapLayer()
    : Layer(kBitmapLayer) {
}

BitmapLayer::~BitmapLayer() {
}

void BitmapLayer::Render(Renderer* renderer, Overlay* overlay,
                         const gfx::Rect& parent_rc) {
  NOTREACHED();
}

void BitmapLayer::SetBounds(const gfx::Rect& bounds) {
}
}  // namespace layers
}  // namespace azer
