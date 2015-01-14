#include "azer/ui/compositor/texture_layer.h"

namespace azer {
namespace compositor {
TextureLayer::TextureLayer(LayerDelegate* delegate)
    : Layer(delegate) {
}

TextureLayer::~TextureLayer() {
}

void TextureLayer::Render(Renderer* renderer) {
}
}  // namespace compositor
}  // namespace azer
