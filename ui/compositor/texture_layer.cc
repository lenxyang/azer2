#include "azer/ui/compositor/texture_layer.h"

namespace azer {
namespace compositor {
TextureLayer::TextureLayer(LayerDelegate* delegate)
    : Layer(delegate) {
}

TextureLayer::~TextureLayer() {
}

}  // namespace compositor
}  // namespace azer
