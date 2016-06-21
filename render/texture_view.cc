#include "azer/render/texture_view.h"

#include "base/logging.h"
#include "azer/render/common.h"
#include "azer/render/texture.h"

namespace azer {
TextureView::Options::Options() 
    : format(kTexFormatUndefined),
      type(TexType::kUnknown) {
}
TextureView::TextureView(const Options& opt, Texture* tex)
    : tex_(tex), 
      options_(opt) {
  DCHECK(tex->options().target & kBindTargetShaderResource);
}

}  // namespace azer
