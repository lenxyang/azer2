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

void RendererLayer::Render(Renderer* renderer, const gfx::Rect& parent_rc) {
}

void RendererLayer::SetBounds(const gfx::Rect& bounds) {
  SetBoundsInternal(bounds);
}

}  // namespace compositor
}  // namespace azer
