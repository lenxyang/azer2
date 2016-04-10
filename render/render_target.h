#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/texture.h"
#include "azer/math/math.h"

namespace azer {

class AZER_EXPORT RenderTarget : public ::base::RefCounted<RenderTarget> {
 public:
  RenderTarget(const Texture::Options& opt, bool default_rt = false);
  virtual ~RenderTarget();

  virtual void Clear(const azer::Vector4& color) = 0;

  Texture* GetTexture() { return texture_.get();}
  const Texture* GetTexture() const { return texture_.get();}
  bool IsDefaultRenderTarget() { return default_render_target_;}

  const gfx::Size& size() const;
  const Texture::Options& options() { return options_;}
 protected:
  const bool default_render_target_;
  TexturePtr texture_;
  const Texture::Options options_;
  DISALLOW_COPY_AND_ASSIGN(RenderTarget);
};

typedef scoped_refptr<RenderTarget> RenderTargetPtr;
}  // namespace azer
