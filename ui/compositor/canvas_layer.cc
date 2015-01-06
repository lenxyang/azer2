#include "azer/ui/compositor/canvas_layer.h"


namespace azer {
namespace ui {

CanvasLayer::CanvasLayer(Layer* parent)
    : Layer(parent) {
}

CanvasLayer::CanvasLayer(LayerTreeHost* host)
    : Layer(host){
}

CanvasLayer::~CanvasLayer() {
}

void CanvasLayer::Render(Renderer* renderer, const gfx::RectF& rect) {
}

}  // namespace ui
}  // namespace azer
