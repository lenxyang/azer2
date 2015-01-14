#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ui/gfx/geometry/rect.h"
#include "azer/base/export.h"

namespace azer {

namespace compositor {
class Layer;

class AZER_EXPORT LayerTreeHostClient {
 public:
  virtual void OnResize(const gfx::Size& size) = 0;
};

class AZER_EXPORT LayerTreeHost {
 public:
  LayerTreeHost();
  ~LayerTreeHost();

  void SetClient(LayerTreeHostClient* client) { client_ = client;}
  void resize(const gfx::Size& size);

  void SetRootLayer(Layer* layer);
 private:
  LayerTreeHostClient* client_;
  Layer* root_;
  DISALLOW_COPY_AND_ASSIGN(LayerTreeHost);
};
}  // namespace compositor
}  // namespace azer
