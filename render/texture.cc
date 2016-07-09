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
      comp_mag_filter(FilterMode::kLinear),
      comp_min_filter(FilterMode::kLinear),
      comp_mip_filter(FilterMode::kLinear),
      compare_func(CompareFunc::kNever),
      border_color(0.0f, 0.0f, 0.0f, 0.0f),
      max_anisotropy(4) {
}

SamplerState::SamplerState(const Options& options)
    : options_(options) {
}

SamplerState::~SamplerState() {}

Texture::Options::Options()
    : format(TexFormat::kRGBA8UNorm),
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
  opt.format = TexFormat::kRGBA8UNorm;
  opt.target = (kBindTargetRenderTarget | kBindTargetShaderResource);
  opt.type = TexType::k2D;
  return opt;
}


const char* GetTexFormatName(TexFormat format) {
  switch (format) {
    case TexFormat::kBGRA8Sint: return "kTexBGRA8Sint";
    case TexFormat::kBGRA8SNorm: return "kTexBGRA8SNorm";
    case TexFormat::kBGRA8Uint: return "kTexBGRA8Uint";
    case TexFormat::kBGRA8UNorm: return "kTexBGRA8UNorm";

    case TexFormat::kRGBA8Sint: return "kTexRGBA8Sint";
    case TexFormat::kRGBA8SNorm: return "kTexRGBA8SNorm";
    case TexFormat::kRGBA8Uint: return "kTexRGBA8Uint";
    case TexFormat::kRGBA8UNorm: return "kTexRGBA8UNorm";

    case TexFormat::kR32Sint: return "kTexR32Sint";
    case TexFormat::kR32Uint: return "kTexR32Uint";
    case TexFormat::kR32Float: return "kTexR32Float";
    case TexFormat::kR32Typeless: return "kTexR32Typeless";

    case TexFormat::kRG32Sint: return "kTexRG32Sint";
    case TexFormat::kRG32Uint: return "kTexRG32Uint";
    case TexFormat::kRG32Float: return "kTexRG32Float";
    case TexFormat::kRG32Typeless: return "kTexRG32Typeless";

    case TexFormat::kRGB32Sint: return "kTexRGB32Sint";
    case TexFormat::kRGB32Uint: return "kTexRGB32Uint";
    case TexFormat::kRGB32Float: return "kTexRGB32Float";
    case TexFormat::kRGB32Typeless: return "kTexRGB32Typeless";

    case TexFormat::kRGBA32Sint: return "kTexRGBA32Sint";
    case TexFormat::kRGBA32Uint: return "kTexRGBA32Uint";
    case TexFormat::kRGBA32Float: return "kTexRGBA32Float";
    case TexFormat::kRGBA32Typeless: return "kTexRGBA32Typeless";

    case TexFormat::kD24UNormS8Uint: return "kTexD24UNormS8Uint";
    case TexFormat::kR24UNormG8Uint: return "kTexR24UNormG8Uint";
    case TexFormat::kR24UNormX8Typeless: return "kTexR24UNormX8Typeless";
    case TexFormat::kX24TypelessG8Uint: return "kTexX24TypelessG8Uint";
    case TexFormat::kD32Float: return "kTexD32Float";

    case TexFormat::kDXBC1n: return "kTexDXBC1n";
    case TexFormat::kDXBC1nSRGB: return "kTexDXBC1nSRGB";
    case TexFormat::kDXBC4s: return "kTexDXBC4s";
    case TexFormat::kDXBC4n: return "kTexDXBC4n";
    case TexFormat::kDXBC2n: return "kTexDXBC2n";
    case TexFormat::kDXBC2nSRGB: return "kTexDXBC2nSRGB";
    case TexFormat::kDXBC3n: return "kTexDXBC3n";
    case TexFormat::kDXBC3nSRGB: return "kTexDXBC3nSRGB";
    case TexFormat::kDXBC5s: return "kTexDXBC5s";
    case TexFormat::kDXBC5n: return "kTexDXBC5n";
    case TexFormat::kDXBC7n: return "kTexDXBC7n";
    case TexFormat::kDXBC7nSRGB: return "kTexDXBC7nSRGB";
    default:
      NOTREACHED();
      return "";
  }
}


uint32_t SizeofTexFormat(TexFormat format) {
  switch (format) {
    case TexFormat::kBGRA8Sint: return 4;
    case TexFormat::kBGRA8SNorm: return 4;
    case TexFormat::kBGRA8Uint: return 4;
    case TexFormat::kBGRA8UNorm: return 4;
    case TexFormat::kRGBA8Sint: return 4;
    case TexFormat::kRGBA8SNorm: return 4;
    case TexFormat::kRGBA8Uint: return 4;
    case TexFormat::kRGBA8UNorm: return 4;
    case TexFormat::kR32Sint: return 4;
    case TexFormat::kR32Uint: return 4;
    case TexFormat::kR32Float: return 4;
    case TexFormat::kR32Typeless: return 4;
    case TexFormat::kRG32Sint: return 8;
    case TexFormat::kRG32Uint: return 8;
    case TexFormat::kRG32Float: return 8;
    case TexFormat::kRG32Typeless: return 8;
    case TexFormat::kRGB32Sint: return 12;
    case TexFormat::kRGB32Uint: return 12;
    case TexFormat::kRGB32Float: return 12;
    case TexFormat::kRGB32Typeless: return 12;
    case TexFormat::kRGBA32Sint: return 16;
    case TexFormat::kRGBA32Uint: return 16;
    case TexFormat::kRGBA32Float: return 16;
    case TexFormat::kRGBA32Typeless: return 16;
    case TexFormat::kD24UNormS8Uint:
    case TexFormat::kR24UNormG8Uint:
    case TexFormat::kR24UNormX8Typeless:
    case TexFormat::kX24TypelessG8Uint:
    case TexFormat::kD32Float:
      return 4;
    case TexFormat::kDXBC1n:
    case TexFormat::kDXBC1nSRGB:
    case TexFormat::kDXBC4n:
    case TexFormat::kDXBC4s:
      return 4;
    case TexFormat::kDXBC2n:
    case TexFormat::kDXBC2nSRGB:
    case TexFormat::kDXBC3n:
    case TexFormat::kDXBC3nSRGB:
    case TexFormat::kDXBC5s:
    case TexFormat::kDXBC5n:
    case TexFormat::kDXBC7n:
    case TexFormat::kDXBC7nSRGB:
      return 8;
    default:
      NOTREACHED();
      return 0;
  }
};

std::ostream& operator << (std::ostream& os, TexFormat format) {
  os << GetTexFormatName(format);
  return os;
}
}   // namespace azer
