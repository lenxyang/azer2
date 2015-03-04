#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/texture.h"
#include "azer/math/math.h"

namespace azer {

class AZER_EXPORT RenderTarget : public ::base::RefCounted<RenderTarget> {
 public:
  RenderTarget(const Texture::Options& opt, bool default_rt = false)
      : default_render_target_(default_rt)
      , options_(opt) {
  }

  virtual ~RenderTarget() {}

  virtual void Clear(const azer::Vector4& color) = 0;

  TexturePtr& GetTexture() { return texture_;}
  const TexturePtr& GetTexture() const { return texture_;}
  bool IsDefaultRenderTarget() { return default_render_target_;}

  const Texture::Options& options() { return options_;}
 protected:
  const bool default_render_target_;
  TexturePtr texture_;
  const Texture::Options options_;
  DISALLOW_COPY_AND_ASSIGN(RenderTarget);
};

typedef scoped_refptr<RenderTarget> RenderTargetPtr;
}  // namespace azer
