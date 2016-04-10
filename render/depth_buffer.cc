#include "azer/render/depth_buffer.h"

namespace azer {
DepthStencilState::DepthStencilState() {
}
DepthStencilState::~DepthStencilState() {
}

const gfx::Size& DepthStencilState::size() const {
  return GetTexture()->size();
}
}  // namespace azer
