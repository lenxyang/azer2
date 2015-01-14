#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/render.h"
#include "azer/ui/compositor/texture_layer.h"

namespace azer {
namespace compositor {
class AZER_EXPORT RendererLayer : public TextureLayer {
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
