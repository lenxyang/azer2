#pragma once

#include "azer/render/texture.h"
#include "azer/render/render_target.h"
#include "azer/render/renderer.h"

namespace azer {
class ImageProcessing {
 public:
  ImageProcessing(TexturePtr input, TexturePtr output);
  ~ImageProcessing();

  void Processing(Effect* effect);
 private:
  void Init();
  TexturePtr input_;
  TexturePtr output_;
  RendererPtr renderer_;
  DISALLOW_COPY_AND_ASSIGN(ImageProcessing);
};
}  // namespace azer
