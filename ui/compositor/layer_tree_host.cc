#include "azer/ui/compositor/layer_tree_host.h"

#include "azer/ui/compositor/layer.h"
#include "azer/ui/compositor/canvas_layer.h"
#include "azer/ui/compositor/nodraw_layer.h"
#include "azer/ui/compositor/renderer_layer.h"

namespace azer {
namespace compositor {

LayerTreeHost::LayerTreeHost(const gfx::Size& size)
    : size_(size)  {
  root_.reset(new NoDrawLayer(this));
}

LayerTreeHost::~LayerTreeHost() {
}

void LayerTreeHost::SetClient(LayerTreeHostClient* client) {
  client_ = client;
  client_->OnResize(size_);
}

void LayerTreeHost::resize(const gfx::Size& size) {
  if (size_ != size) {
    size_= size;
    root_->SetBounds(gfx::Rect(0, 0, size.width(), size.height()));
    if (client_) {
      client_->OnResize(size);
    }
  }
}

}  // namespace compositor
}  // namespace azer
