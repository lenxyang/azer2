#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/layers/texture_layer.h"
#include "azer/render/canvas2d.h"

namespace azer {

class Overlay;
class Renderer;

namespace layers {

class LayerDelegate;

class AZER_EXPORT NoDrawLayer : public Layer {
 public:
  explicit NoDrawLayer() : Layer(kNotDrawnLayer) {}
  ~NoDrawLayer() override {}

  void SetBounds(const gfx::Rect& bounds) override {
    SetBoundsInternal(bounds);
  }

  void Render(Renderer* renderer, Overlay* overlay,
              const gfx::Rect& parent_rc) override {}
  void Redraw() override {}
 private:
  DISALLOW_COPY_AND_ASSIGN(NoDrawLayer);
};

}  // namespace layers
}  // namespace azer
