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

Blending::Blending(const Desc& desc) 
    : enable_alpha_to_converage_(false),
      enable_independent_blending_(false),
      desc_(desc) {
}

Blending::~Blending() {}

void Blending::EnableAlphaToConverage(bool enable) {
  if (enable_alpha_to_converage_ != enable) {
    enable_alpha_to_converage_ = enable;
    CHECK(OnDescChanged());
  }
}

void Blending::EnableIndependentBlend(bool enable) {
  if (enable_independent_blending_ != enable) {
    enable_independent_blending_ = enable;
    CHECK(OnDescChanged());
  }
}
}  // namespace azer
