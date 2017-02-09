#include "azer/render/texture.h"

#include "third_party/skia/include/core/SkBitmap.h"

#include "base/files/file_path.h"
#include "azer/base/image.h"
#include "azer/render/render_system.h"
#include "azer/render/canvas2d.h"

namespace azer {

Texture::Options::Options()
    : format(TexFormat::kRGBA8UNorm),
      usage(kBufferDefault),
      cpu_access(kCPUNoAccess),
      target(kBindTargetUnknown),
      mipmap_level(1),
      genmipmap(false) {
}

// TexSize
TexSize::TexSize(const gfx::Size& size) {
  this->width = size.width();
  this->height = size.height();
  this->depth = 1;
}

TexSize::TexSize(int width, int height, int depth) {
  this->width = width;
  this->height = height;
  this->depth = depth;
}

TexSize::TexSize() {
  width = height = 0;
  depth = 1;
}

bool operator != (const TexSize& t1, const TexSize& t2) {
  return !(t1 == t2);
}

bool operator == (const TexSize& t1, const TexSize& t2) {
  return t1.width == t2.width
      && t1.height == t2.height
      && t1.depth == t2.depth;
}

namespace {
GpuResOptions FromTexOptions(const Texture::Options& opt) {
  GpuResOptions o;
  o.usage = opt.usage;
  o.cpu_access = opt.cpu_access;
  o.target = opt.target;
  o.type = GpuResType::kTexture;
  return o;
}
}

Texture::Texture(const Options& opt, TexType type, int diminison) 
    : GpuResource(FromTexOptions(opt)),
      options_(opt),
      type_(type),
      diminison_(diminison) {
}

const TexSize& Texture::size() const {
  return options_.size;
}

bool Texture::Save(const ::base::FilePath& path) {
  SkBitmap bitmap;
  SkImageInfo info = SkImageInfo::Make(options_.size.width,
                                       options_.size.height,
                                       kRGBA_8888_SkColorType,
                                       kOpaque_SkAlphaType);
  bitmap.setInfo(info);
  bitmap.allocPixels();

  uint8_t* pixels = (uint8_t*)bitmap.getPixels();
  GpuResLockDataPtr dataptr = this->map(MapType::kWriteDiscard);
  if (dataptr->data_ptr() == NULL) {
    return false;
  }

  uint32_t row_pitch = dataptr->row_size();
  for (int32_t i = 0; i < options_.size.height; ++i) {
    memcpy(pixels, dataptr->data_ptr() + i * row_pitch, row_pitch);
    pixels += row_pitch;
  }
  this->unmap();
  return SaveSkBitmap(bitmap, path);
}

Texture::Options InitTexOptForRenderTarget(const gfx::Size& size) {
  Texture::Options opt;
  opt.size = TexSize(size.width(), size.height(), 1);
  opt.genmipmap = true;
  opt.format = TexFormat::kRGBA8UNorm;
  opt.target = (kBindTargetRenderTarget | kBindTargetShaderResource);
  return opt;
}

// class Texture2D
Texture2D::Texture2D(const Options& options)
    : Texture(options, TexType::k2D, 1) {
}

// class Texture2DArray
Texture2DArray::Texture2DArray(const Options& options, int dim)
    : Texture(options, TexType::k2DArray, dim) {
}

// class TextureCubemap
TextureCubemap::TextureCubemap(const Options& options)
    : Texture(options, TexType::kCubemap, 6) {
}

// class Texture3D
Texture3D::Texture3D(const Options& options)
    : Texture(options, TexType::k3D, 1) {
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

    case TexFormat::kR16Sint: return "kR16Sint";
    case TexFormat::kR16SNorm: return "kR16SNorm";
    case TexFormat::kR16Uint: return "kR16Uint";
    case TexFormat::kR16UNorm: return "kR16UNorm";
    case TexFormat::kR16Float: return "kR16Float";
    case TexFormat::kR16Typeless: return "kR16Typeless";
    case TexFormat::kRG16Sint: return "kRG16Sint";
    case TexFormat::kRG16SNorm: return "kRG16SNorm";
    case TexFormat::kRG16Uint: return "kRG16Uint";
    case TexFormat::kRG16UNorm: return "kRG16UNorm";
    case TexFormat::kRG16Float: return "kRG16Float";
    case TexFormat::kRG16Typeless: return "kRG16Typeless";
    case TexFormat::kRGB16Sint: return "kRGB16Sint";
    case TexFormat::kRGB16SNorm: return "kRGB16SNorm";
    case TexFormat::kRGB16Uint: return "kRGB16Uint";
    case TexFormat::kRGB16UNorm: return "kRGB16UNorm";
    case TexFormat::kRGB16Float: return "kRGB16Float";
    case TexFormat::kRGB16Typeless: return "kRGB16Typeless";
    case TexFormat::kRGBA16Sint: return "kRGBA16Sint";
    case TexFormat::kRGBA16SNorm: return "kRGBA16SNorm";
    case TexFormat::kRGBA16Uint: return "kRGBA16Uint";
    case TexFormat::kRGBA16UNorm: return "kRGBA16UNorm";
    case TexFormat::kRGBA16Float: return "kRGBA16Float";
    case TexFormat::kRGBA16Typeless: return "kRGBA16Typeless";

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
    case TexFormat::kR16Sint:
    case TexFormat::kR16SNorm:
    case TexFormat::kR16Uint:
    case TexFormat::kR16UNorm:
    case TexFormat::kR16Float:
    case TexFormat::kR16Typeless:
      return 2;
    case TexFormat::kRG16Sint:
    case TexFormat::kRG16SNorm:
    case TexFormat::kRG16Uint:
    case TexFormat::kRG16UNorm:
    case TexFormat::kRG16Float:
    case TexFormat::kRG16Typeless:
      return 4;
    case TexFormat::kRGB16Sint:
    case TexFormat::kRGB16SNorm:
    case TexFormat::kRGB16Uint:
    case TexFormat::kRGB16UNorm:
    case TexFormat::kRGB16Float:
    case TexFormat::kRGB16Typeless:
      return 6;
    case TexFormat::kRGBA16Sint:
    case TexFormat::kRGBA16SNorm:
    case TexFormat::kRGBA16Uint:
    case TexFormat::kRGBA16UNorm:
    case TexFormat::kRGBA16Float:
    case TexFormat::kRGBA16Typeless:
      return 8;
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
