#include "azer/render/layers/layer_tree_host.h"

#include "azer/render/layers/layer.h"
#include "azer/render/layers/canvas_layer.h"
#include "azer/render/layers/nodraw_layer.h"
#include "azer/render/layers/renderer_layer.h"
#include "azer/render/layers/compositor.h"
#include "azer/render/render_system.h"

namespace azer {
namespace layers {

LayerTreeHost::LayerTreeHost(const gfx::Size& size)
    : layers_(NULL)
    , client_(NULL)
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

void LayerTreeHost::SetCompositor(Compositor* layers) {
  DCHECK(NULL != root());
  client_ = layers;
  layers_ = layers;

  UpdateResourceHierarchy(root());
  if (layers) {
    client_->OnResize(size_);
    SetLayerNeedRedrawHierarchy(root());
  }
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

void LayerTreeHost::UpdateResourceHierarchy(Layer* layer) {
  layer->SetBounds(layer->bounds());
  for (auto iter = layer->children_.begin();
       iter != layer->children_.end();
       ++iter) {
    (*iter)->SetBounds((*iter)->bounds());
  }
}

void LayerTreeHost::SetLayerNeedRedraw(Layer* layer) {
  DCHECK(layers());
  layers()->AddNeedRedrawLayer(layer);
}

void LayerTreeHost::SetLayerNeedRedrawHierarchy(Layer* layer) {
  DCHECK(layers());
  layers()->AddNeedRedrawLayer(layer);
  
  for (auto iter = layer->children().begin();
       iter != layer->children().end();
       ++iter) {
    SetLayerNeedRedrawHierarchy(*iter);
  }
}

Compositor* LayerTreeHost::layers() {
  return layers_;
}

void LayerTreeHost::OnLayerAttachedOnTree(Layer* layer) {
  if (layers()) {
    layers()->AddNeedRedrawLayer(layer);
  }
}

void LayerTreeHost::OnLayerResized(Layer* layer) {
  if (layers()) {
    layers()->AddNeedRedrawLayer(layer);
  }
}

void LayerTreeHost::OnLayerDestroying(Layer* layer) {
  if (layers()) {
    layers()->TryRemoveNeedRedrawLayer(layer);
  }
}
}  // namespace layers
}  // namespace azer
