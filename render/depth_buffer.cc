#include "azer/render/depth_buffer.h"

#include "azer/render/texture.h"
#include "azer/render/render_system.h"

namespace azer {
DepthStencilState::DepthStencilState() {
}
DepthStencilState::~DepthStencilState() {
}

DepthBuffer::DepthBuffer(const Options& opt) 
    : options_(opt) {
}

DepthBuffer::~DepthBuffer() {
}

const gfx::Size& DepthBuffer::size() const {
  return GetTexture()->size();
}
}  // namespace azer
