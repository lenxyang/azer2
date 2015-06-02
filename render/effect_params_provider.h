#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {

class Effect;

// provider information for effect
class AZER_EXPORT EffectParamsProvider
    : public base::RefCounted<EffectParamsProvider> {
 public:
  EffectParamsProvider();
  virtual ~EffectParamsProvider();

  virtual const char* name() const = 0;

  // initialize
  virtual void CalcParams() = 0;

  // apply the parameters on effect
  virtual void Apply(Effect* effect) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(EffectParamsProvider);
};

}  // namespace azer
