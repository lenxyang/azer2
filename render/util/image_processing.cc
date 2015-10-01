#include "azer/render/util/image_processing.h"

#include "azer/render/renderer.h"
#include "azer/render/render_system.h"

namespace azer {
ImageProcessing::ImageProcessing(TexturePtr input, TexturePtr output)
    : input_(input),
      output_(output) {
}

ImageProcessing::~ImageProcessing() {
}

void ImageProcessing::Processing(Effect* effect) {
}

void ImageProcessing::Init() {
  RenderSystem* rs = RenderSystem::Current();
  Texture::Options rdopt;
  rdopt.size = output_->options().size;
  rdopt.target = (Texture::BindTarget)
      (Texture::kRenderTarget | Texture::kShaderResource);
  renderer_ = rs->CreateRenderer(rdopt);
  viewport_.bounds = gfx::Rect(rdopt.size);
  renderer_->SetViewport(viewport_);
}
}  // namespace azer
