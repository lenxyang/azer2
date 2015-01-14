#include "azer/ui/compositor/renderer_layer.h"

#include "azer/render/render.h"
#include "base/logging.h"

namespace azer {
namespace compositor {

RendererLayer::RendererLayer(LayerDelegate* delegate)
    : TextureLayer(delegate) {
}

RendererLayer::~RendererLayer() {
}

void RendererLayer::Render(Renderer* renderer, const gfx::RectF& rect) {
}

}  // namespace compositor
}  // namespace azer
