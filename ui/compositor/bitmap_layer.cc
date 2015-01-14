#include "azer/ui/compositor/bitmap_layer.h"

namespace azer {
namespace compositor {
BitmapLayer::BitmapLayer(Layer* parent)
    : Layer(parent) {
}

BitmapLayer::~BitmapLayer() {
}

void BitmapLayer::Render(Renderer* renderer) {
}
}  // namespace compositor
}  // namespace azer
