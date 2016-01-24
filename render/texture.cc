#include "azer/render/texture.h"

#include "third_party/skia/include/core/SkBitmap.h"

#include "base/files/file_path.h"
#include "azer/base/image.h"
#include "azer/render/render_system.h"
#include "azer/render/canvas2d.h"

namespace azer {
SamplerState::SamplerState()
    : wrap_u(kTexAddrModeWrap)
    , wrap_v(kTexAddrModeWrap)
    , wrap_w(kTexAddrModeWrap)
    , mag_filter(kFilterModeLinear)
    , min_filter(kFilterModeLinear)
    , mip_filter(kFilterModeLinear)
    , compare_func(kCompareFuncNever)
    , border_color(0.0f, 0.0f, 0.0f, 0.0f)
    , mip_level(1)
    , max_anisotropy(4)
    , sample_level(1)
    , sample_quality(0) {
}

Texture::Options::Options()
    : format(kRGBAn8),
      usage(GraphicBuffer::kDefault),
      cpu_access(kCPUNoAccess),
      target(kBindTargetUnknown),
      type(kTex2D),
      diminison(1),
      genmipmap(false) {
}

Texture::Texture(const Options& opt) 
    : Resource(kTexture),
      options_(opt) {
}

bool Texture::Save(const ::base::FilePath& path) {
  SkBitmap bitmap;
  SkImageInfo info = SkImageInfo::Make(options_.size.width(),
                                       options_.size.height(),
                                       kRGBA_8888_SkColorType,
                                       kOpaque_SkAlphaType);
  bitmap.setInfo(info);
  bitmap.allocPixels();

  uint8* pixels = (uint8*)bitmap.getPixels();
  MapData mapdata = map(kReadOnly);
  if (mapdata.pdata == NULL) {
    return false;
  }

  uint32 row_pitch = mapdata.row_pitch;
  for (int32 i = 0; i < options_.size.height(); ++i) {
    memcpy(pixels, mapdata.pdata + i * row_pitch, row_pitch);
    pixels += row_pitch;
  }
  unmap();
  return SaveSkBitmap(bitmap, path);
}

Texture::Options InitTexOptForRenderTarget(const gfx::Size& size) {
  Texture::Options opt;
  opt.size = size;
  opt.genmipmap = true;
  opt.format = kRGBAn8;
  opt.target = (kBindTargetRenderTarget | kBindTargetShaderResource);
  opt.type = kTex2D;
  // opt.sampler.mip_level = -1;
  opt.sampler.mip_level = 1;
  return opt;
}
}   // namespace azer
