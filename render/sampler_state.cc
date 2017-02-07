#include "azer/render/sampler_state.h"

namespace azer {

SampleDesc::SampleDesc()
    : count(1), quality(0) {
}

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
}  // namespace azer
