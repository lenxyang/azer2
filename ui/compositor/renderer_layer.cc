#include "azer/ui/compositor/renderer_layer.h"

#include "azer/render/render.h"
#include "base/logging.h"

namespace azer {
namespace ui {

RendererLayer::RendererLayer(Layer* parent)
    : Layer(parent) {
}

RendererLayer::RendererLayer(LayerTreeHost* host)
    : Layer(host){
}

RendererLayer::~RendererLayer() {
}

void RendererLayer::Render(Renderer* renderer, const gfx::RectF& rect) {
}

}  // namespace ui
}  // namespace azer
