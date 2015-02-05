#include "azer/render/layers/layer_tree_host.h"

#include "azer/render/layers/layer.h"
#include "azer/render/layers/canvas_layer.h"
#include "azer/render/layers/nodraw_layer.h"
#include "azer/render/layers/renderer_layer.h"
#include "azer/render/render_system.h"

namespace azer {
namespace layers {

LayerTreeHost::LayerTreeHost(const gfx::Size& size)
    : client_(NULL)
    , root_(NULL)
    , size_(size)  {
}

LayerTreeHost::~LayerTreeHost() {
}

void LayerTreeHost::SetRoot(scoped_refptr<Layer> layer) {
  DCHECK(!root_);
  root_ = layer;
  root_->host_ = this;
  layer->SetBounds(gfx::Rect(size_));
}

void LayerTreeHost::SetClient(LayerTreeHostClient* client) {
  DCHECK(NULL != root());
  client_ = client;
}

void LayerTreeHost::resize(const gfx::Size& size) {
  RenderSystem* rs = RenderSystem::Current();
  if (size_ != size) {
    size_= size;
    if (root_) {
      root_->SetBounds(gfx::Rect(0, 0, size.width(), size.height()));
    }

    if (client_) {
      client_->OnResize(size);
    }
  }
}

void LayerTreeHost::SetLayerNeedRedraw(Layer* layer) {
  DCHECK(client_);
  client_->OnLayerNeedRedraw(layer);
}

void LayerTreeHost::SetLayerNeedRedrawHierarchy(Layer* layer) {
  DCHECK(client_);
  client_->OnLayerNeedRedraw(layer);
  
  for (auto iter = layer->children().begin();
       iter != layer->children().end();
       ++iter) {
    SetLayerNeedRedrawHierarchy(*iter);
  }
}

void LayerTreeHost::OnLayerAttachedOnTree(Layer* layer) {
  if (client_) {
    client_->OnLayerAttached(layer);
  }
}

void LayerTreeHost::OnLayerResized(Layer* layer) {
  if (client_) {
    client_->OnLayerNeedRedraw(layer);
  }
}

void LayerTreeHost::OnLayerDestroying(Layer* layer) {
  if (client_) {
    client_->OnLayerRemoved(layer);
  }
}
}  // namespace layers
}  // namespace azer
