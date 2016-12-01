#include "azer/render/depth_buffer.h"

#include "azer/render/texture.h"
#include "azer/render/render_system.h"

namespace azer {
DepthBuffer::Options::Options() 
    : type(TexType::kUnknown),
      format(TexFormat::kUndefined),
      depth_readonly(false),
      stencil_readonly(false) {
}

DepthStencilState::DepthStencilState() {}
DepthStencilState::~DepthStencilState() {}

// class DepthBuffer
DepthBuffer::DepthBuffer(const Options& opt) 
    : ResourceView(ResViewType::kDepthBuffer),
      options_(opt) {
}

DepthBuffer::~DepthBuffer() {}

gfx::Size DepthBuffer::size() const {
  return gfx::Size(GetTexture()->size().width, GetTexture()->size().height);
}
}  // namespace azer
