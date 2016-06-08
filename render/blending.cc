#include "azer/render/blending.h"

#include "base/logging.h"
#include "azer/render/renderer.h"

namespace azer {
ScopedResetBlending::ScopedResetBlending(Renderer* renderer)
    : renderer_(renderer) {
}

ScopedResetBlending::~ScopedResetBlending() {
  renderer_->ResetBlending();
}

Blending::Blending(const BlendDesc& desc) 
    : desc_(desc) {
}

Blending::~Blending() {}

Blending::RenderTargetDesc::RenderTargetDesc()
    : src(kOne),
      dest(kZero),
      oper(kAdd),
      src_alpha(kOne),
      dest_alpha(kZero),
      alpha_oper(kAdd),
      mask(kWriteAll),
      enable(false) {
}

Blending::BlendDesc::BlendDesc()
    : alpha_to_converage(false),
      independent_blending(false) {
}
}  // namespace azer
