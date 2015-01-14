#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/render.h"
#include "azer/ui/compositor/layer.h"

class SkBitmap;

namespace azer {
namespace compositor {

class BitmapLayer : public Layer {
 public:
  explicit BitmapLayer(LayerDelegate* delegate);
  ~BitmapLayer() override;

  void Render(Renderer* renderer, const gfx::Rect& parent_rc) override;
  void Redraw() override {}
  void SetBounds(const gfx::Rect& bounds) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(BitmapLayer);
};
}  // namespace compositor
}  // namespace azer
