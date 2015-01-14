#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/render.h"
#include "azer/ui/compositor/layer.h"

namespace azer {
namespace compositor {
class AZER_EXPORT RendererLayer : public Layer {
 public:
  explicit RendererLayer(LayerDelegate* delegate);

  ~RendererLayer() override;

  void Render(Renderer* renderer) override;
 private:
  RendererPtr renderer_;
  DISALLOW_COPY_AND_ASSIGN(RendererLayer);
};
}  // namespace compositor
}  // namespace azer
