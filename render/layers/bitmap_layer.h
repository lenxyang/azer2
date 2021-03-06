#pragma once


#include "azer/base/export.h"
#include "azer/azer.h"
#include "azer/render/layers/layer.h"

class SkBitmap;

namespace azer {
namespace layers {

class BitmapLayer : public Layer {
 public:
  explicit BitmapLayer();
  ~BitmapLayer() override;

  void Render(Renderer* renderer, Overlay* overlay,
              const gfx::Rect& parent_rc) override;
  void Redraw() override {}
  void SetBounds(const gfx::Rect& bounds) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(BitmapLayer);
};
}  // namespace layers
}  // namespace azer
