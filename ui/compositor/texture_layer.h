#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/render.h"
#include "azer/render/blending.h"
#include "azer/render/texture.h"
#include "azer/ui/compositor/layer.h"

namespace azer {
namespace compositor {
class TextureLayer : public Layer {
 public:
  explicit TextureLayer(LayerDelegate* delegate);
  ~TextureLayer() override;

  void Render(Renderer* renderer) override;
  void SetBlending(BlendingPtr& ptr) { blending_ = ptr;}
  TexturePtr& GetContent() { return texture_;}
 private:
  BlendingPtr blending_;
  TexturePtr texture_;
  DISALLOW_COPY_AND_ASSIGN(TextureLayer);
};
}  // namespace compositor
}  // namespace azer
