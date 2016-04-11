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
    : format(kTexRGBAn8),
      usage(kBufferDefault),
      cpu_access(kCPUNoAccess),
      target(kBindTargetUnknown),
      type(kTex2D),
      diminison(1),
      genmipmap(false) {
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
  opt.format = kTexRGBAn8;
  opt.target = (kBindTargetRenderTarget | kBindTargetShaderResource);
  opt.type = kTex2D;
  // opt.sampler.mip_level = -1;
  opt.sampler.mip_level = 1;
  return opt;
}


const char* GetTexFormatName(TexFormat format) {
  switch (format) {
    case kTexRGBA8: return "rgba(8)";
    case kTexRGBAn8: return "rgba(norm8)";
    case kTexRGBA32: return "rgba(32)";
    case kTexRGBAn32:return "rgba(norm32)";
    case kTexRGBAf: return "rgba(f)";
    case kTexR11G11B10f: return "r11g11b10";
    case kTexDepth24nStencil8u: return "Depth24nStencil8u";
    case kTexR24G8: return "kTexR24G8";
    case kTexR24nX8: return "kTexR24nX8";
    case kTexDXBC1n: return "DXBC1n";
    case kTexDXBC1nSRGB: return "DXBC1nSRGB";
    case kTexDXBC4s: return "DXBC4s";
    case kTexDXBC4n: return "DXBC4n";
    case kTexDXBC2n: return "DXBC2n";
    case kTexDXBC2nSRGB: return "DXBC2nSRGB";
    case kTexDXBC3n: return "DXBC3n";
    case kTexDXBC3nSRGB: return "DXBC3nSRGB";
    case kTexDXBC5s: return "DXBC5s";
    case kTexDXBC5n: return "DXBC5n";
    case kTexDXBC7n: return "DXBC7n";
    case kTexDXBC7nSRGB: return "DXBC7nSRGB";
    default:
      NOTREACHED();
      return "";
  }
}


uint32 SizeofTexFormat(TexFormat format) {
  switch (format) {
    case kTexRGBA8: return sizeof(int8) * 4;
    case kTexRGBAn8: return sizeof(uint8) * 4;
    case kTexRGBA32: return sizeof(int32) * 4;
    case kTexRGBAn32: return sizeof(uint32) * 4;
    case kTexRGBAf: return sizeof(float) * 4;
    case kTexDepth24nStencil8u:
    case kTexR24G8:
    case kTexR24nX8:
      return sizeof(uint32);
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
