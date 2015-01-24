#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/render.h"
#include "azer/render/compositor/texture_layer.h"

namespace azer {
namespace compositor {
class AZER_EXPORT RendererLayer : public TextureLayer {
 public:
  explicit RendererLayer();

  ~RendererLayer() override;

  void Redraw() override;
  void SetBounds(const gfx::Rect& bounds) override;
 private:
  RendererPtr renderer_;
  DISALLOW_COPY_AND_ASSIGN(RendererLayer);
};
}  // namespace compositor
}  // namespace azer
