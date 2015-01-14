#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/ui/compositor/texture_layer.h"
#include "azer/render/canvas2d.h"

namespace azer {
namespace compositor {

class AZER_EXPORT CanvasLayer : public TextureLayer {
 public:
  explicit CanvasLayer(LayerDelegate* delegate);

  ~CanvasLayer() override;

  void Render(Renderer* renderer, const gfx::Rect& parent_rc) override;
 private:
  Canvas2DPtr canvas_;
  DISALLOW_COPY_AND_ASSIGN(CanvasLayer);
};

}  // namespace compositor
}  // namespace azer
