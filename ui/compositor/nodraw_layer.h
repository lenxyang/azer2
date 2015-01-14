#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/ui/compositor/texture_layer.h"
#include "azer/render/canvas2d.h"

namespace azer {
namespace compositor {

class LayerDelegate;

class AZER_EXPORT NoDrawLayer : public Layer {
 public:
  explicit NoDrawLayer(LayerDelegate* delegate) : Layer(delegate) {}
  ~NoDrawLayer() override {}

  void Render(Renderer* renderer, const gfx::Rect& parent_rc) override {}
 private:
  DISALLOW_COPY_AND_ASSIGN(NoDrawLayer);
};

}  // namespace compositor
}  // namespace azer
