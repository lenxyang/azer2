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

DepthBufferPtr CreateDepthBufferForSurface(Surface* surface) {
  RenderSystem* rs = RenderSystem::Current();
  Texture::Options o;
  o.format = kTexR24G8;
  o.target = kBindTargetDepthStencil | kBindTargetShaderResource;
  o.size = surface->GetBounds().size();
  o.sample_desc.count = surface->sample_desc().count;
  o.sample_desc.quality = surface->sample_desc().quality;
  o.genmipmap = false;
  TexturePtr tex = rs->CreateTexture(o);
  return rs->CreateDepthBuffer(DepthBuffer::Options(), tex);
}
}  // namespace azer
