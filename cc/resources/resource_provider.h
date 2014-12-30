#pragma once

#include <map>

#include "azer/render/texture.h"
#include "base/basictypes.h"
#include "cc/resources/resource_provider.h"
#include "cc/base/cc_export.h"

namespace azer {
class RenderSystem;
}  // namespace azer

namespace cc {
class CC_EXPORT AzerResourceProvider : public ResourceProvider {
 public:
  ResourceId CreateResource(const gfx::Size& size, 
                            GLint wrap_mode,
                            TextureHint hint,
                            ResourceFormat format) override;

  void DeleteResource(ResourceId id) override;

  void SetPixels(ResourceId id,
                 const uint8_t* image,
                 const gfx::Rect& image_rect,
                 const gfx::Rect& source_rect,
                 const gfx::Vector2d& dest_offset) override;
 private:
  ResourceId CreateTexture(const gfx::Size& size,
                           GLenum target,
                           GLenum texture_pool,
                           GLint wrap_mode,
                           TextureHint hint,
                           ResourceFormat format);
  
  azer::RenderSystem* render_system_;
  typedef std::map<ResourceId, azer::TexturePtr> TextureMap;
  TextureMap texture_map_;
  DISALLOW_COPY_AND_ASSIGN(AzerResourceProvider);
};
}  // namespace cc
