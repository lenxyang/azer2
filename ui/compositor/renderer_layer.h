#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/render.h"
#include "azer/ui/compositor/layer.h"

namespace azer {
namespace ui {
class AZER_EXPORT RendererLayer : public Layer {
 public:
  explicit RendererLayer(Layer* parent);
  explicit RendererLayer(LayerTreeHost* host);

  ~RendererLayer() override;

  void Render(Renderer* renderer, const gfx::RectF& rect) override;
 private:
  RendererPtr renderer_;
  DISALLOW_COPY_AND_ASSIGN(RendererLayer);
};
}  // namespace ui
}  // namespace azer
