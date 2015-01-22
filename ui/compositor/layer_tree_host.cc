#include "azer/ui/compositor/layer_tree_host.h"

#include "azer/ui/compositor/layer.h"
#include "azer/ui/compositor/canvas_layer.h"
#include "azer/ui/compositor/nodraw_layer.h"
#include "azer/ui/compositor/renderer_layer.h"
#include "azer/ui/compositor/compositor.h"
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
  root_->host_ = this;
  size_ = root_->size();
}

void LayerTreeHost::SetCompositor(Compositor* compositor) {
  DCHECK(NULL != root());
  client_ = compositor;
  compositor_ = compositor;
  if (compositor) {
    client_->OnResize(size_);
    SetLayerNeedRedrawHierarchy(root());
  }
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

void LayerTreeHost::SetLayerNeedRedrawHierarchy(Layer* layer) {
  if (compositor()) {
    compositor()->AddNeedRedrawLayer(layer);
  }

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
