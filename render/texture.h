#pragma once

#include <memory>

#include "ui/gfx/geometry/size.h"
#include "base/files/file_path.h"
#include "azer/math/math.h"
#include "azer/base/export.h"
#include "azer/base/resource.h"
#include "azer/base/string.h"
#include "azer/render/render_system_enum.h"

namespace azer {

class RenderSystem;
class Image;

class AZER_EXPORT Texture : public Resource {
public:
  enum BindTarget {
    kUnknown        = 0,
    kShaderResource = 0x8L,
    kRenderTarget   = 0x20L,
    kDepthStencil   = 0x40L,
  };

  enum WrapMode {
    kWrap = 1,
    kMirror,
    kClamp,
    kBorder,
    kMirrorOnce,
  };

  enum FilterMode {
    kPoint = 1,
    kLinear,
  };

  enum Type {
    k1D,
    k1DArray,
    k2D,
    k2DArray,
    k2DMultiSample,
    k2DArrayMultiSample,
    k3D,
    kCubemap,
    kCubemapArray,
  };

  struct AZER_EXPORT SamplerState {
    Texture::WrapMode wrap_u;
    Texture::WrapMode wrap_v;
    Texture::WrapMode wrap_w;
    Texture::FilterMode mag_filter;
    Texture::FilterMode min_filter;
    Texture::FilterMode mip_filter;
    CompareFunc compare_func;
    Vector4 border_color;
    int mip_level;
    int max_anisotropy;
    int sample_level;
    int sample_quality;
 
    SamplerState();
  };

  struct AZER_EXPORT Options {
    gfx::Size size;
    SamplerState sampler;
    DataFormat format;            // default: kRGBAn8
    GraphicBuffer::Usage usage;   // default: GraphicBuffer::kDefault
    CPUAccess cpu_access;         // default: kCPUNoAccess
    Texture::BindTarget target;
    Type type;
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

  virtual MapData map(MapType maptype) = 0;
  virtual void unmap() = 0;
  virtual bool InitFromImage(const Image* image) = 0;

  virtual bool CopyTo(Texture* texture) = 0;
  
  // save the texture into file
  // for debug
  bool Save(const ::base::FilePath& path);

  const Options& options() const { return options_;}
protected:
  Options options_;
  DISALLOW_COPY_AND_ASSIGN(Texture);
};

typedef scoped_refptr<Texture> TexturePtr;


AZER_EXPORT Texture::Options InitTexOptForRenderTarget(const gfx::Size& size);
}  // namespace azer
