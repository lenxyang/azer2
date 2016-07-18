#pragma once

#include <memory>
#include <ostream>

#include "ui/gfx/geometry/size.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "azer/math/math.h"
#include "azer/base/export.h"
#include "azer/base/string.h"
#include "azer/render/common.h"
#include "azer/render/gpu_resource.h"

namespace azer {

class RenderSystem;
class ImageData;
class SamplerState;
class Texture;
typedef scoped_refptr<Texture> TexturePtr;
typedef scoped_refptr<SamplerState> SamplerStatePtr;

enum class TexFormat {
  kUndefined = 0,
  kBGRA8Sint,
  kBGRA8SNorm,
  kBGRA8Uint,
  kBGRA8UNorm,
  kRGBA8Sint,
  kRGBA8SNorm,
  kRGBA8Uint,
  kRGBA8UNorm,
  kR16Sint,
  kR16SNorm,
  kR16Uint,
  kR16UNorm,
  kR16Float,
  kR16Typeless,
  kRG16Sint,
  kRG16SNorm,
  kRG16Uint,
  kRG16UNorm,
  kRG16Float,
  kRG16Typeless,
  kRGB16Sint,
  kRGB16SNorm,
  kRGB16Uint,
  kRGB16UNorm,
  kRGB16Float,
  kRGB16Typeless,
  kRGBA16Sint,
  kRGBA16SNorm,
  kRGBA16Uint,
  kRGBA16UNorm,
  kRGBA16Float,
  kRGBA16Typeless,
  kR32Sint,
  kR32Uint,
  kR32Float,
  kR32Typeless,
  kRG32Sint,
  kRG32Uint,
  kRG32Float,
  kRG32Typeless,
  kRGB32Sint,
  kRGB32Uint,
  kRGB32Float,
  kRGB32Typeless,
  kRGBA32Sint,
  kRGBA32Uint,
  kRGBA32Float,
  kRGBA32Typeless,
  kR11G11B10f,
  kD24UNormS8Uint,
  kR24UNormG8Uint,
  kR24UNormX8Typeless,
  kX24TypelessG8Uint,
  kD32Float,
  kDXBC1n = 1000,
  kDXBC1nSRGB,
  kDXBC4s,
  kDXBC4n,
  kDXBC2n,
  kDXBC2nSRGB,
  kDXBC3n,
  kDXBC3nSRGB,
  kDXBC5s,
  kDXBC5n,
  kDXBC7n,
  kDXBC7nSRGB,
};

struct AZER_EXPORT SampleDesc {
  int count;
  int quality;

  SampleDesc();
};

class AZER_EXPORT SamplerState : public ::base::RefCounted<SamplerState> {
 public:
  struct AZER_EXPORT Options {
    TexAddressMode wrap_u;
    TexAddressMode wrap_v;
    TexAddressMode wrap_w;
    FilterMode mag_filter;
    FilterMode min_filter;
    FilterMode mip_filter;
    CompareFunc compare_func;
    FilterMode comp_mag_filter;
    FilterMode comp_min_filter;
    FilterMode comp_mip_filter;
    Vector4 border_color;
    int max_anisotropy;
 
    Options();
  };

  explicit SamplerState(const Options& state);
  virtual ~SamplerState();
  const Options& options() const { return options_;}
 protected:
  virtual bool Init() = 0;
 private:
  const Options options_;
  DISALLOW_COPY_AND_ASSIGN(SamplerState);
};

class AZER_EXPORT Texture : public GpuResource {
 public:
  struct AZER_EXPORT Options {
    gfx::Size size;
    SampleDesc sample_desc;
    TexFormat format;            // default: kRGBAn8
    BufferUsage usage;   // default: GraphicBuffer::kDefault
    CPUAccess cpu_access;         // default: kCPUNoAccess
    uint32_t target;
    TexType type;
    int32_t diminison;
    int32_t mipmap_level;
    bool genmipmap;
    Options();
  };

  explicit Texture(const Options& opt);
  virtual ~Texture() {}

  const gfx::Size& size() const;
  
  // save the texture into file
  // for debug
  bool Save(const ::base::FilePath& path);
  const Options& options() const { return options_;}
 protected:
  Options options_;
  DISALLOW_COPY_AND_ASSIGN(Texture);
};

AZER_EXPORT uint32_t SizeofTexFormat(TexFormat format);
AZER_EXPORT std::ostream& operator << (std::ostream& os, TexFormat format);
AZER_EXPORT Texture::Options InitTexOptForRenderTarget(const gfx::Size& size);
}  // namespace azer
