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

  struct SamplerState {
    Texture::WrapMode wrap_u;
    Texture::WrapMode wrap_v;
    Texture::WrapMode wrap_w;
    Texture::FilterMode mag_filter;
    Texture::FilterMode min_filter;
    Texture::FilterMode mip_filter;
    CompareFunc::Type compare_func;
    Vector4 border_color;
    int mip_level;
    int max_anisotropy;
    int sample_level;
    int sample_qualifiy;

    SamplerState()
        : wrap_u(Texture::kWrap)
        , wrap_v(Texture::kWrap)
        , wrap_w(Texture::kWrap)
        , mag_filter(Texture::kLinear)
        , min_filter(Texture::kLinear)
        , mip_filter(Texture::kLinear)
        , compare_func(CompareFunc::kNever)
        , border_color(0.0f, 0.0f, 0.0f, 0.0f)
        , mip_level(1)
        , max_anisotropy(1)
        , sample_level(1)
        , sample_qualifiy(0) {
    }
  };

  struct Options {
    gfx::Size size;
    SamplerState sampler;
    DataFormat format;
    GraphicBuffer::Usage usage;
    CPUAccess cpu_access;
    Texture::BindTarget target;
    Type type;

    Options()
        : format(kRGBAn8)
        , usage(GraphicBuffer::kDefault)
        , cpu_access(kCPUNoAccess)
        , target(Texture::kUnknown)
        , type(k2D) {
    }
  };

  explicit Texture(const Options& opt);
  virtual ~Texture() {}
  
  // not opt's width and height will be ignored
  static Texture* Load(const Options& opt, const ::base::FilePath& path,
                       RenderSystem* rs);
  static Texture* Load(const Options& opt, const StringType& path,
                       RenderSystem* rs);
  static Texture* Load(Type type, const ::base::FilePath& path,
                       RenderSystem* rs);
  static Texture* Load(Type type, const StringType& path, RenderSystem* rs);

  const Options& option() const { return options_;}

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
protected:
  Options options_;
  DISALLOW_COPY_AND_ASSIGN(Texture);
};

typedef std::shared_ptr<Texture> TexturePtr;

inline Texture::Texture(const Options& opt) 
    : Resource(kTexture)
    , options_(opt) {
}

inline Texture* Texture::Load(const Options& opt, const StringType& path,
                              RenderSystem* rs) {
  return Load(opt, ::base::FilePath(path), rs);
}

inline Texture* Texture::Load(Type type, const StringType& path,
                              RenderSystem* rs) {
  return Load(type, ::base::FilePath(path), rs);
}
}  // namespace azer
