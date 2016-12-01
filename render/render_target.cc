#include "azer/render/render_target.h"

namespace azer {
RenderTarget::Options::Options() 
    : format(TexFormat::kUndefined) {
  memset(name, 0, sizeof(name));
}

RenderTarget::RenderTarget(const Options& opt, bool default_rt)
    : ResourceView(ResViewType::kRenderTarget),
      default_render_target_(default_rt),
      options_(opt) {
}

RenderTarget::~RenderTarget() {
}

gfx::Size RenderTarget::size() const { 
  return gfx::Size(texture_->size().width, texture_->size().height);
}
}  // namespace azer
