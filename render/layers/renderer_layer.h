#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/layers/texture_layer.h"
#include "azer/render/renderer.h"

namespace azer {
namespace layers {
class AZER_EXPORT RendererLayer : public TextureLayer {
 public:
  explicit RendererLayer();

  ~RendererLayer() override;

  void Redraw() override;
  void SetBounds(const gfx::Rect& bounds) override;
 protected:
  void OnBoundsChanged() override;
  RendererPtr renderer_;
  DISALLOW_COPY_AND_ASSIGN(RendererLayer);
};
}  // namespace layers
}  // namespace azer
