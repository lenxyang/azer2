#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/render.h"
#include "azer/render/blending.h"
#include "azer/render/texture.h"
#include "azer/render/layers/layer.h"

namespace gfx {
class RectF;
class Rect;
}

namespace azer {

class Renderer;

namespace layers {

class AZER_EXPORT TextureLayer : public Layer {
 public:
  explicit TextureLayer(LayerType type);
  ~TextureLayer() override;

  void SetBlending(BlendingPtr& ptr) { blending_ = ptr;}

  void Render(Renderer* renderer, Overlay* overlay,
              const gfx::Rect& parent_rc) override;
  TexturePtr& GetContent() { return texture_;}
 protected:
  void UpdateOverlayCoord();
 protected:
  BlendingPtr blending_;
  TexturePtr texture_;
  DISALLOW_COPY_AND_ASSIGN(TextureLayer);
};
}  // namespace layers
}  // namespace azer
