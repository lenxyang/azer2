#include "azer/render/texture_view.h"

#include "base/logging.h"

namespace azer {
TextureView::Options::Options() 
    : format(kTexFormatUndefined),
      type(kUnkonwnTexType) {
}
TextureView::TextureView(const Options::opt, Texture* tex)
    : texture_(tex), 
      options_(opt) {
  DCHECK(texture->options().target & kBindTargetShaderResource);
  DCHECK(CheckTexFormatCapability());
}

void TextureView::GenerateMips(int level) {
  CHECK(false) << "GenerateMips should not called";
}
}  // namespace azer
