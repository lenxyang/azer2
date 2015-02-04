#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/compositor/texture_layer.h"
#include "azer/render/canvas2d.h"

namespace azer {
namespace compositor {

class LayerDelegate;

class AZER_EXPORT NoDrawLayer : public Layer {
 public:
  explicit NoDrawLayer() : Layer(kNotDrawnLayer) {}
  ~NoDrawLayer() override {}

  void SetBounds(const gfx::Rect& bounds) override {
    SetBoundsInternal(bounds);
  }

  void Render(Renderer* renderer, const gfx::Rect& parent_rc) override {}
  void Redraw() override {}
 private:
  DISALLOW_COPY_AND_ASSIGN(NoDrawLayer);
};

}  // namespace compositor
}  // namespace azer
