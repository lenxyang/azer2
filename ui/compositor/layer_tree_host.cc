#include "azer/ui/compositor/layer_tree_host.h"

#include "azer/ui/compositor/layer.h"
#include "azer/ui/compositor/canvas_layer.h"
#include "azer/ui/compositor/renderer_layer.h"

namespace azer {
namespace compositor {
LayerTreeHost::LayerTreeHost(RenderSystem* rs)
    : render_system_(rs) {
}

scoped_refptr<Layer> LayerTreeHost::CreateCanvasLayer(const gfx::Rect& rect) {
  DCHECK(NULL != root());
  return scoped_refptr<Layer>(new CanvasLayer(root()));
}

scoped_refptr<Layer> LayerTreeHost::CreateRendererLayer(const gfx::Rect& rect) {
  DCHECK(NULL != root());
  return scoped_refptr<Layer>(new RendererLayer(root()));
}
}  // namespace compositor
}  // namespace azer
