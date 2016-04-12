#pragma once

#include <memory>

#include "ui/gfx/geometry/size.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "azer/math/math.h"
#include "azer/base/export.h"
#include "azer/base/string.h"
#include "azer/render/common.h"

namespace azer {

class RenderSystem;
class ImageData;
class Texture;
typedef scoped_refptr<Texture> TexturePtr;

enum TexFormat {
  kUndefined = 0,
  kTexBGRAn8,
  kTexRGBA8,
  kTexRGBAn8,
  kTexR32,
  kTexRG32,
  kTexRGB32,
  kTexRGBA32,
  kTexRGBAn32,
  kTexRGBAf,
  kTexR11G11B10f,
  kTexDepth24nStencil8u,
  kTexR24G8,
  kTexR24nX8,
  kTexDXBC1n = 1000,
  kTexDXBC1nSRGB,
  kTexDXBC4s,
  kTexDXBC4n,
  kTexDXBC2n,
  kTexDXBC2nSRGB,
  kTexDXBC3n,
  kTexDXBC3nSRGB,
  kTexDXBC5s,
  kTexDXBC5n,
  kTexDXBC7n,
  kTexDXBC7nSRGB,
};

struct AZER_EXPORT SamplerState {
  AddressMode wrap_u;
  AddressMode wrap_v;
  AddressMode wrap_w;
  FilterMode mag_filter;
  FilterMode min_filter;
  FilterMode mip_filter;
  CompareFunc compare_func;
  Vector4 border_color;
  int mip_level;
  int max_anisotropy;
  int sample_level;
  int sample_quality;
 
  SamplerState();
};

class AZER_EXPORT Texture : public ::base::RefCounted<Texture> {
 public:
  struct AZER_EXPORT Options {
    gfx::Size size;
    SamplerState sampler;
    TexFormat format;            // default: kRGBAn8
    BufferUsage usage;   // default: GraphicBuffer::kDefault
    CPUAccess cpu_access;         // default: kCPUNoAccess
    uint32 target;
    TexType type;
    int32 diminison;
    bool genmipmap;
    Options();
  };

  explicit Texture(const Options& opt);
  virtual ~Texture() {}
  
  virtual bool SetSamplerState(const SamplerState& sampler_state) = 0;
  virtual void GenerateMips(int level) = 0; 

  /**
   * Note: map 返回 MapData 结构体，返回的数据当中 width 和 depth
   * 不一定和 option 当中制定的大小是一致的
   * 导致这个问题的原因是，有可能 GPU 采用更大的对其方式
   * 因此操作内存时，应以 MapData 给定的数据为准，不要使用 Options 的数据
   */
  struct MapData {
    uint8* pdata;
    uint32 row_pitch;
    uint32 depth_pitch;
  };


  const gfx::Size& size() const;
  virtual MapData map(MapType maptype) = 0;
  virtual void unmap() = 0;
  virtual bool InitFromImage(const ImageData* image) = 0;

  virtual bool CopyTo(Texture* texture) = 0;
  
  // save the texture into file
  // for debug
  bool Save(const ::base::FilePath& path);

  const Options& options() const { return options_;}
 protected:
  Options options_;
  DISALLOW_COPY_AND_ASSIGN(Texture);
};

AZER_EXPORT Texture::Options InitTexOptForRenderTarget(const gfx::Size& size);
}  // namespace azer
