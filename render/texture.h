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
#include "azer/render/sampler_state.h"

namespace azer {

class RenderSystem;
class ImageData;

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

struct AZER_EXPORT TexSize {
  int width;
  int height;
  int depth;

  TexSize();
  TexSize(const gfx::Size& size);
  TexSize(int width, int height, int depth);
};

AZER_EXPORT bool operator == (const TexSize& t1, const TexSize& t2);
AZER_EXPORT bool operator != (const TexSize& t1, const TexSize& t2);

struct AZER_EXPORT Texture2DArraySlice {
  int first_slice;
  int array_size;
  int mip_slice;

  Texture2DArraySlice()
      : first_slice(0), array_size(0), mip_slice(0) {}
  Texture2DArraySlice(int first, int size, int mip)
      : first_slice(first), array_size(size), mip_slice(mip) {}
};

class AZER_EXPORT Texture : public GpuResource {
 public:
  struct AZER_EXPORT Options {
    TexSize size;
    SampleDesc sample_desc;
    TexFormat format;            // default: kRGBAn8
    BufferUsage usage;   // default: GraphicBuffer::kDefault
    CPUAccess cpu_access;         // default: kCPUNoAccess
    uint32_t target;
    int mipmap_level;
    bool genmipmap;
    Options();
  };
protected:
  Texture(const Options& opt, TexType type, int diminison);
public:
  virtual ~Texture() {}

  const TexSize& size() const;
  TexType type() const { return type_;}
  int diminison() const { return diminison_;}
  
  // save the texture into file
  // for debug
  bool Save(const ::base::FilePath& path);
  const Options& options() const { return options_;}
  virtual bool Init(const ImageData* image) = 0;
 protected:
  Options options_;
  TexType type_;
  int diminison_;
  DISALLOW_COPY_AND_ASSIGN(Texture);
};

class AZER_EXPORT Texture1D : public Texture {
 public:
  explicit Texture1D(const Options& options);
 private:
  DISALLOW_COPY_AND_ASSIGN(Texture1D);
};

class AZER_EXPORT Texture1DArray : public Texture {
 public:
  explicit Texture1DArray(const Options& options, int dim);
 private:
  DISALLOW_COPY_AND_ASSIGN(Texture1DArray);
};

class AZER_EXPORT Texture2D : public Texture {
 public:
  explicit Texture2D(const Options& options);
 private:
  DISALLOW_COPY_AND_ASSIGN(Texture2D);
};

class AZER_EXPORT Texture2DArray : public Texture {
 public:
  Texture2DArray(const Options& options, int diminison);
 private:
  DISALLOW_COPY_AND_ASSIGN(Texture2DArray);
};

class AZER_EXPORT Texture3D : public Texture {
 public:
  Texture3D(const Options& options);
 private:
  DISALLOW_COPY_AND_ASSIGN(Texture3D);
};

class AZER_EXPORT TextureCubemap : public Texture {
 public:
  explicit TextureCubemap(const Options& options);
 private:
  DISALLOW_COPY_AND_ASSIGN(TextureCubemap);
};

typedef scoped_refptr<Texture> TexturePtr;
typedef scoped_refptr<Texture2D> Texture2DPtr;
typedef scoped_refptr<Texture2DArray> Texture2DArrayPtr;
typedef scoped_refptr<TextureCubemap> TextureCubemapPtr;
typedef scoped_refptr<Texture3D> Texture3DPtr;

AZER_EXPORT uint32_t SizeofTexFormat(TexFormat format);
AZER_EXPORT std::ostream& operator << (std::ostream& os, TexFormat format);
AZER_EXPORT Texture::Options InitTexOptForRenderTarget(const gfx::Size& size);
}  // namespace azer
