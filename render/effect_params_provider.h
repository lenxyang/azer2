#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/effect.h"

namespace azer {

class FrameArgs;

// provider information for effect
class AZER_EXPORT EffectParamsProvider : public base::RefCounted<EffectParamsProvider> {
 public:
  EffectParamsProvider();
  virtual ~EffectParamsProvider();

  // initialize
  virtual bool Init() = 0;

  // apply the parameters on effect
  virtual void ApplyParametersForEffect() = 0;

  // GetEffect
  virtual EffectPtr GetEffect() = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(EffectParamsProvider);
};

}  // namespace azer
