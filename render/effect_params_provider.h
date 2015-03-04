#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {

class FrameData;

// provider information for effect
class AZER_EXPORT EffectParamsProvider {
 public:
  EffectParamsProvider();
  virtual ~EffectParamsProvider();

  // apply the parameters on effect
  virtual ApplyParametersForEffect(Effect* effect) = 0;

  // GetEffect
  virtual EffectPtr& GetEffect() = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(EffectParamsProvider);
};

}  // namespace azer
