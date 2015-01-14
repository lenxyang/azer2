#include "azer/ui/compositor/layer_tree_host.h"

#include "azer/ui/compositor/layer.h"
#include "azer/ui/compositor/canvas_layer.h"
#include "azer/ui/compositor/nodraw_layer.h"
#include "azer/ui/compositor/renderer_layer.h"
#include "azer/render/render_system.h"

namespace azer {
namespace compositor {

namespace {
void DeepDeleteLayers(Layer* layer) {
  LayerList children = layer->children();
  for (auto it = children.begin(); it != children.end(); ++it) {
    Layer* child = *it;
    DeepDeleteLayers(child);
  }
  delete layer;
}
}  // namespace

LayerTreeHost::LayerTreeHost(const gfx::Size& size)
    : size_(size)
    , compositor_(NULL) {
  root_ = new NoDrawLayer(this);
  root_->host_ = this;
}

LayerTreeHost::~LayerTreeHost() {
  if (root_) {
    DeepDeleteLayers(root_);
  }
}

void LayerTreeHost::SetClient(LayerTreeHostClient* client) {
  client_ = client;
  client_->OnResize(size_);
}

void LayerTreeHost::resize(const gfx::Size& size) {
  RenderSystem* rs = RenderSystem::Current();
  if (size_ != size) {
    size_= size;
    root_->SetBounds(gfx::Rect(0, 0, size.width(), size.height()));
    if (client_) {
      client_->OnResize(size);
    }
  }
}

Compositor* LayerTreeHost::compositor() {
  DCHECK(NULL != compositor_);
  return compositor_;
}

}  // namespace compositor
}  // namespace azer
