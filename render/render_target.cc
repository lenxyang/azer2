#include "azer/render/render_target.h"

namespace azer {
RenderTarget::RenderTarget(const Texture::Options& opt, bool default_rt)
    : default_render_target_(default_rt),
      options_(opt) {
}

RenderTarget::~RenderTarget() {
}

const gfx::Size& RenderTarget::size() const { 
  return texture_->size();
}
}  // namespace azer
