#include "azer/ui/compositor/layer_tree_host.h"

#include "azer/ui/compositor/layer.h"
#include "azer/ui/compositor/canvas_layer.h"
#include "azer/ui/compositor/renderer_layer.h"

namespace azer {
namespace compositor {
LayerTreeHost::LayerTreeHost() {
}

LayerTreeHost::~LayerTreeHost() {
}

void LayerTreeHost::SetRootLayer(Layer* layer) {
  DCHECK(NULL == root_);
  root_ = layer;
  layer->SetTreeHost(this);
}

}  // namespace compositor
}  // namespace azer
