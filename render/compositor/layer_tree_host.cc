#include "azer/render/compositor/layer_tree_host.h"

#include "azer/render/compositor/layer.h"
#include "azer/render/compositor/canvas_layer.h"
#include "azer/render/compositor/nodraw_layer.h"
#include "azer/render/compositor/renderer_layer.h"
#include "azer/render/compositor/compositor.h"
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
    : compositor_(NULL)
    , client_(NULL)
    , root_(NULL)
    , size_(size)  {
}

LayerTreeHost::~LayerTreeHost() {
  if (root_) {
    DeepDeleteLayers(root_);
  }
}

void LayerTreeHost::SetRoot(Layer* layer) {
  DCHECK(!root_);
  root_ = layer;
  root_->host_ = this;
  layer->SetBounds(gfx::Rect(size_));
}

void LayerTreeHost::SetCompositor(Compositor* compositor) {
  DCHECK(NULL != root());
  client_ = compositor;
  compositor_ = compositor;

  UpdateResourceHierarchy(root());
  if (compositor) {
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
  DCHECK(compositor());
  compositor()->AddNeedRedrawLayer(layer);
}

void LayerTreeHost::SetLayerNeedRedrawHierarchy(Layer* layer) {
  DCHECK(compositor());
  compositor()->AddNeedRedrawLayer(layer);
  
  for (auto iter = layer->children().begin();
       iter != layer->children().end();
       ++iter) {
    SetLayerNeedRedrawHierarchy(*iter);
  }
}

Compositor* LayerTreeHost::compositor() {
  return compositor_;
}

void LayerTreeHost::OnLayerAttachedOnTree(Layer* layer) {
  if (compositor()) {
    compositor()->AddNeedRedrawLayer(layer);
  }
}

void LayerTreeHost::OnLayerResized(Layer* layer) {
  if (compositor()) {
    compositor()->AddNeedRedrawLayer(layer);
  }
}

void LayerTreeHost::OnLayerDestroying(Layer* layer) {
  if (compositor()) {
    compositor()->TryRemoveNeedRedrawLayer(layer);
  }
}
}  // namespace compositor
}  // namespace azer
