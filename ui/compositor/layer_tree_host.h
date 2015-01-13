#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ui/gfx/geometry/rect.h"
#include "azer/base/export.h"

namespace azer {

class RenderSystem;

namespace compositor {
class Layer;

class AZER_EXPORT LayerTreeHostClient {
 public:
  virtual void OnResize(const gfx::Size& size) = 0;
};

class AZER_EXPORT LayerTreeHost {
 public:
  explicit LayerTreeHost(RenderSystem* rs);
  ~LayerTreeHost();

  Layer* root() { return root_.get();}

  void SetClient(LayerTreeHostClient* client) { client_ = client;}
  void resize(const gfx::Size& size);

  // create specify type layer
  scoped_refptr<Layer> CreateCanvasLayer(const gfx::Rect& rect);
  scoped_refptr<Layer> CreateRendererLayer(const gfx::Rect& rect);

  RenderSystem* GetRenderSystem() { return render_system_;}
 private:
  scoped_refptr<Layer> root_;
  LayerTreeHostClient* client_;
  RenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(LayerTreeHost);
};
}  // namespace compositor
}  // namespace azer
