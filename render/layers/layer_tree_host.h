#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ui/gfx/geometry/rect.h"

#include "azer/base/export.h"
#include "azer/render/render.h"
#include "azer/render/render_system_observer.h"
#include "azer/render/compositor/layer_delegate.h"
#include "azer/render/compositor/layer_observer.h"


namespace azer {

namespace compositor {
class Compositor;
class Layer;

class AZER_EXPORT LayerTreeHostClient {
 public:
  virtual void OnResize(const gfx::Size& size) = 0;
};

class AZER_EXPORT LayerTreeHost : public LayerObserver {
 public:
  explicit LayerTreeHost(const gfx::Size& size);
  ~LayerTreeHost();

  void SetCompositor(Compositor* compositor);
  void resize(const gfx::Size& size);

  Layer* root() { return root_;}
  const Layer* root() const { return root_;}
  void SetRoot(scoped_refptr<Layer> layer);

  Compositor* compositor();
  void SetLayerNeedRedrawHierarchy(Layer* layer);
  void SetLayerNeedRedraw(Layer* layer);
 private:
  void UpdateResourceHierarchy(Layer* layer);
  // LayerObserver for all layer in the tree
  void OnLayerAttachedOnTree(Layer* layer) override;
  void OnLayerResized(Layer* layer) override;
  void OnLayerDestroying(Layer* layer) override;

  Compositor* compositor_;
  LayerTreeHostClient* client_;
  scoped_refptr<Layer> root_;
  gfx::Size size_;
  friend class Compositor;
  DISALLOW_COPY_AND_ASSIGN(LayerTreeHost);
};
}  // namespace compositor
}  // namespace azer
