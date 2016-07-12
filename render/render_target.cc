#include "azer/render/render_target.h"

namespace azer {
RenderTarget::Options::Options() 
    : format(TexFormat::kUndefined) {
}

RenderTarget::RenderTarget(const Options& opt, bool default_rt)
    : ResourceView(ResViewType::kRenderTarget),
      default_render_target_(default_rt),
      options_(opt) {
}

RenderTarget::~RenderTarget() {
}

const gfx::Size& RenderTarget::size() const { 
  return texture_->size();
}
}  // namespace azer
