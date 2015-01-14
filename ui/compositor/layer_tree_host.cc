#include "azer/ui/compositor/layer_tree_host.h"

#include "azer/ui/compositor/layer.h"
#include "azer/ui/compositor/canvas_layer.h"
#include "azer/ui/compositor/nodraw_layer.h"
#include "azer/ui/compositor/renderer_layer.h"

namespace azer {
namespace compositor {

LayerTreeHost::LayerTreeHost() {
  root_ = new NoDrawLayer(this);
}

LayerTreeHost::~LayerTreeHost() {
}

void LayerTreeHost::SetClient(LayerTreeHostClient* client) {
  client_ = client;
  client_->OnResize(root()->bounds()->size());
}

void LayerTreeHost::resize(const gfx::Size& size) {
  if (root_->bounds().size() != size) {
    root_->SetBounds(gfx::Rect(0, 0, size));
    if (client_) {
      client_->OnResize(size);
    }
  }
}

}  // namespace compositor
}  // namespace azer
