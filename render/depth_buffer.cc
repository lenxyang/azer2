#include "azer/render/depth_buffer.h"

#include "azer/render/texture.h"

namespace azer {
DepthStencilState::DepthStencilState() {
}
DepthStencilState::~DepthStencilState() {
}

const gfx::Size& DepthBuffer::size() const {
  return GetTexture()->size();
}
}  // namespace azer
