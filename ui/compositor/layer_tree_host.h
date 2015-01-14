#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ui/gfx/geometry/rect.h"

#include "azer/base/export.h"
#include "azer/render/render.h"
#include "azer/render/render_system_observer.h"
#include "azer/ui/compositor/layer_delegate.h"


namespace azer {

namespace compositor {
class Layer;

class AZER_EXPORT LayerTreeHostClient {
 public:
  virtual void OnResize(const gfx::Size& size) = 0;
};

class AZER_EXPORT LayerTreeHost : public LayerDelegate {
 public:
  LayerTreeHost(const gfx::Size& size);
  ~LayerTreeHost();

  void SetClient(LayerTreeHostClient* client);
  void resize(const gfx::Size& size);

  Layer* root() { return root_;}
  const Layer* root() const { return root_;}

  OverlayPtr& overlay() { return overlay_;}
 private:
  void OnPaintLayer(gfx::Canvas* canvas) override {}

  LayerTreeHostClient* client_;
  Layer* root_;
  gfx::Size size_;
  // all texture merge, use it
  OverlayPtr overlay_;
  DISALLOW_COPY_AND_ASSIGN(LayerTreeHost);
};
}  // namespace compositor
}  // namespace azer
