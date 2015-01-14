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
  explicit BitmapLayer(Layer* parent);
  ~BitmapLayer() override;

  void Render(Renderer* renderer) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(BitmapLayer);
};
}  // namespace compositor
}  // namespace azer
