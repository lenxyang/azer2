#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/layers/texture_layer.h"
#include "azer/render/canvas2d.h"

namespace azer {
namespace layers {

class AZER_EXPORT CanvasLayer : public TextureLayer {
 public:
  explicit CanvasLayer();

  ~CanvasLayer() override;

  void Redraw() override;
  void SetBounds(const gfx::Rect& bounds) override;
 protected:
  void OnBoundsChanged() override;
  Canvas2DPtr canvas_;
  DISALLOW_COPY_AND_ASSIGN(CanvasLayer);
};

}  // namespace layers
}  // namespace azer
