#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/ui/compositor/layer.h"
#include "azer/render/canvas2d.h"

namespace azer {
namespace ui {

class AZER_EXPORT CanvasLayer : public Layer {
 public:
  explicit CanvasLayer(Layer* parent);
  explicit CanvasLayer(LayerTreeHost* host);

  ~CanvasLayer() override;

  void Render(Renderer* renderer, const gfx::RectF& rect) override;
 private:
  Canvas2DPtr canvas_;
  DISALLOW_COPY_AND_ASSIGN(CanvasLayer);
};

}  // namespace ui
}  // namespace azer
