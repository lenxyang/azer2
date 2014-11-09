#include "azer/render/texture.h"

#include "base/files/file_path.h"
#include "azer/base/image.h"
#include "azer/render/render_system.h"

namespace azer {
Texture* Texture::Load(Type type, const ::base::FilePath& path, RenderSystem* rs) {
  Texture::Options opt;
  opt.target = Texture::kShaderResource;
  opt.type = (Type)type;
  return Load(opt, path, rs);
}

Texture* Texture::Load(const Options& o, const ::base::FilePath& path,
                       RenderSystem* rs) {
  std::unique_ptr<Image> imgptr(Image::Load(path, (Image::Type)o.type));
  if (imgptr.get()) {
    Texture::Options opt = o;
    opt.width = imgptr->width();
    opt.height = imgptr->height();
    return rs->CreateTexture(opt, imgptr.get());
  } else {
    LOG(ERROR) << "failed to load texture: \"" << path.value() << "\"";
    return NULL;
  }
}
}   // namespace azer
