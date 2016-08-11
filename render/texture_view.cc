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
TextureUAView::Options::Options() 
    : format(TexFormat::kUndefined),
      type(TexType::kUnknown) {
}
TextureUAView::TextureUAView(const Options& opt, Texture* tex)
    : UnorderAccessView(ResViewType::kTexture, tex),
      tex_(tex), 
      options_(opt) {
  DCHECK(tex->options().target & kBindTargetShaderResource);
}

}  // namespace azer
