#include "azer/render/texture_view.h"

#include "base/logging.h"
#include "azer/render/common.h"
#include "azer/render/texture.h"

namespace azer {
TextureView::Options::Options() 
    : format(TexFormat::kUndefined),
      type(TexType::kUnknown) {
}
TextureView::TextureView(const Options& opt, Texture* tex)
    : ShaderResView(ResViewType::kTexture, tex),
      tex_(tex), 
      options_(opt) {
  DCHECK(tex->options().target & kBindTargetShaderResource);
}

// class Options
UATextureView::Options::Options() 
    : format(TexFormat::kUndefined),
      type(TexType::kUnknown) {
}
UATextureView::UATextureView(const Options& opt, Texture* tex)
    : UnorderAccessResView(ResViewType::kTexture, tex),
      tex_(tex), 
      options_(opt) {
  DCHECK(tex->options().target & kBindTargetShaderResource);
}

}  // namespace azer
