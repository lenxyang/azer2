#include "azer/render/blending.h"

#include "azer/render/renderer.h"

namespace azer {
ScopedResetBlending::ScopedResetBlending(Renderer* renderer)
    : renderer_(renderer) {
}

ScopedResetBlending::~ScopedResetBlending() {
  renderer_->ResetBlending();
}

}  // namespace azer
