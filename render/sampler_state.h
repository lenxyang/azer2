#pragma once

#include "base/memory/ref_counted.h"
#include "azer/math/vector4.h"
#include "azer/render/common.h"
#include "azer/render/gpu_resource.h"

namespace azer {

class SamplerState;
typedef scoped_refptr<SamplerState> SamplerStatePtr;

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

}  // namespace azer
