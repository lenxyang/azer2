#include "azer/render/texture.h"

#include "third_party/skia/include/core/SkBitmap.h"

#include "base/files/file_path.h"
#include "azer/base/image.h"
#include "azer/render/render_system.h"
#include "azer/render/canvas2d.h"

namespace azer {
SamplerState::Options::Options()
    : wrap_u(TexAddressMode::kWrap),
      wrap_v(TexAddressMode::kWrap),
      wrap_w(TexAddressMode::kWrap),
      mag_filter(FilterMode::kLinear),
      min_filter(FilterMode::kLinear),
      mip_filter(FilterMode::kLinear),
      compare_func(CompareFunc::kNever),
      border_color(0.0f, 0.0f, 0.0f, 0.0f),
      max_anisotropy(4) {
}

SamplerState::SamplerState(const Options& options)
    : options_(options) {
}

SamplerState::~SamplerState() {}

Texture::Options::Options()
    : format(kTexRGBA8UNorm),
      usage(kBufferDefault),
      cpu_access(kCPUNoAccess),
      target(kBindTargetUnknown),
      type(TexType::k2D),
      diminison(1),
      mipmap_level(1),
      genmipmap(false) {
}

SampleDesc::SampleDesc()
    : count(1), quality(0) {
}

Texture::Texture(const Options& opt) : options_(opt) {}

const gfx::Size& Texture::size() const {
  return options_.size;
}

bool Texture::Save(const ::base::FilePath& path) {
  SkBitmap bitmap;
  SkImageInfo info = SkImageInfo::Make(options_.size.width(),
                                       options_.size.height(),
                                       kRGBA_8888_SkColorType,
                                       kOpaque_SkAlphaType);
  bitmap.setInfo(info);
  bitmap.allocPixels();

  uint8_t* pixels = (uint8_t*)bitmap.getPixels();
  MapData mapdata = map(MapType::kReadOnly);
  if (mapdata.pdata == NULL) {
    return false;
  }

  uint32_t row_pitch = mapdata.row_pitch;
  for (int32_t i = 0; i < options_.size.height(); ++i) {
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
  opt.format = kTexRGBA8UNorm;
  opt.target = (kBindTargetRenderTarget | kBindTargetShaderResource);
  opt.type = TexType::k2D;
  return opt;
}


const char* GetTexFormatName(TexFormat format) {
  switch (format) {
    case kTexBGRA8Sint: return "kTexBGRA8Sint";
    case kTexBGRA8SNorm: return "kTexBGRA8SNorm";
    case kTexBGRA8Uint: return "kTexBGRA8Uint";
    case kTexBGRA8UNorm: return "kTexBGRA8UNorm";

    case kTexRGBA8Sint: return "kTexRGBA8Sint";
    case kTexRGBA8SNorm: return "kTexRGBA8SNorm";
    case kTexRGBA8Uint: return "kTexRGBA8Uint";
    case kTexRGBA8UNorm: return "kTexRGBA8UNorm";

    case kTexR32Sint: return "kTexR32Sint";
    case kTexR32Uint: return "kTexR32Uint";
    case kTexR32Float: return "kTexR32Float";
    case kTexR32Typeless: return "kTexR32Typeless";

    case kTexRG32Sint: return "kTexRG32Sint";
    case kTexRG32Uint: return "kTexRG32Uint";
    case kTexRG32Float: return "kTexRG32Float";
    case kTexRG32Typeless: return "kTexRG32Typeless";

    case kTexRGB32Sint: return "kTexRGB32Sint";
    case kTexRGB32Uint: return "kTexRGB32Uint";
    case kTexRGB32Float: return "kTexRGB32Float";
    case kTexRGB32Typeless: return "kTexRGB32Typeless";

    case kTexRGBA32Sint: return "kTexRGBA32Sint";
    case kTexRGBA32Uint: return "kTexRGBA32Uint";
    case kTexRGBA32Float: return "kTexRGBA32Float";
    case kTexRGBA32Typeless: return "kTexRGBA32Typeless";

    case kTexD24UNormS8Uint: return "kTexD24UNormS8Uint";
    case kTexR24UNormG8Uint: return "kTexR24UNormG8Uint";
    case kTexR24UNormX8Typeless: return "kTexR24UNormX8Typeless";
    case kTexX24TypelessG8Uint: return "kTexX24TypelessG8Uint";
    case kTexD32Float: return "kTexD32Float";

    case kTexDXBC1n: return "kTexDXBC1n";
    case kTexDXBC1nSRGB: return "kTexDXBC1nSRGB";
    case kTexDXBC4s: return "kTexDXBC4s";
    case kTexDXBC4n: return "kTexDXBC4n";
    case kTexDXBC2n: return "kTexDXBC2n";
    case kTexDXBC2nSRGB: return "kTexDXBC2nSRGB";
    case kTexDXBC3n: return "kTexDXBC3n";
    case kTexDXBC3nSRGB: return "kTexDXBC3nSRGB";
    case kTexDXBC5s: return "kTexDXBC5s";
    case kTexDXBC5n: return "kTexDXBC5n";
    case kTexDXBC7n: return "kTexDXBC7n";
    case kTexDXBC7nSRGB: return "kTexDXBC7nSRGB";
    default:
      NOTREACHED();
      return "";
  }
}


uint32_t SizeofTexFormat(TexFormat format) {
  switch (format) {
    case kTexBGRA8Sint: return 4;
    case kTexBGRA8SNorm: return 4;
    case kTexBGRA8Uint: return 4;
    case kTexBGRA8UNorm: return 4;
    case kTexRGBA8Sint: return 4;
    case kTexRGBA8SNorm: return 4;
    case kTexRGBA8Uint: return 4;
    case kTexRGBA8UNorm: return 4;
    case kTexR32Sint: return 4;
    case kTexR32Uint: return 4;
    case kTexR32Float: return 4;
    case kTexR32Typeless: return 4;
    case kTexRG32Sint: return 8;
    case kTexRG32Uint: return 8;
    case kTexRG32Float: return 8;
    case kTexRG32Typeless: return 8;
    case kTexRGB32Sint: return 12;
    case kTexRGB32Uint: return 12;
    case kTexRGB32Float: return 12;
    case kTexRGB32Typeless: return 12;
    case kTexRGBA32Sint: return 16;
    case kTexRGBA32Uint: return 16;
    case kTexRGBA32Float: return 16;
    case kTexRGBA32Typeless: return 16;
    case kTexD24UNormS8Uint:
    case kTexR24UNormG8Uint:
    case kTexR24UNormX8Typeless:
    case kTexX24TypelessG8Uint:
    case kTexD32Float:
      return 4;
    case kTexDXBC1n:
    case kTexDXBC1nSRGB:
    case kTexDXBC4n:
    case kTexDXBC4s:
      return 4;
    case kTexDXBC2n:
    case kTexDXBC2nSRGB:
    case kTexDXBC3n:
    case kTexDXBC3nSRGB:
    case kTexDXBC5s:
    case kTexDXBC5n:
    case kTexDXBC7n:
    case kTexDXBC7nSRGB:
      return 8;
    default:
      NOTREACHED();
      return 0;
  }
};
}   // namespace azer
