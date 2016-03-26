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

}  // namespace azer
