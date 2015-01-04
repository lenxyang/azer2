#include "azer/render/texture.h"

#include "third_party/skia/include/core/SkBitmap.h"

#include "base/files/file_path.h"
#include "azer/base/image.h"
#include "azer/render/render_system.h"
#include "azer/render/canvas2d.h"

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

bool Texture::Save(const ::base::FilePath& path) {
  SkBitmap bitmap;
  SkImageInfo info = SkImageInfo::Make(options_.width,
                                       options_.height,
                                       kRGBA_8888_SkColorType,
                                       kOpaque_SkAlphaType);
  bitmap.setInfo(info);
  bitmap.allocPixels();

  uint8* pixels = (uint8*)bitmap.getPixels();
  MapData mapdata = map(kReadOnly);
  uint32 row_pitch = mapdata.row_pitch;
  for (int32 i = 0; i < options_.height; ++i) {
    memcpy(pixels, mapdata.pdata + i * row_pitch, row_pitch);
    pixels += row_pitch;
  }
  unmap();
  return SaveSkBitmap(bitmap, path);
}
}   // namespace azer
