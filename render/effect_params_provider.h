#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {

class Effect;
class EffectParamsProvider;
class FrameArgs;

class AZER_EXPORT EffectParamsAdapter {
 public:
  static const char kEffectParamsAdapterName[];
  EffectParamsAdapter();
  virtual ~EffectParamsAdapter();

  virtual const char* name() const { return kEffectParamsAdapterName;}
  virtual void Apply(Effect* effect, EffectParamsProvider* params) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(EffectParamsAdapter);
};

// provider information for effect
class AZER_EXPORT EffectParamsProvider : 
      public ::base::RefCounted<EffectParamsProvider> {
 public:
  static const char kEffectParamsProviderName[];
  EffectParamsProvider();
  virtual ~EffectParamsProvider();

  virtual const char* name() const { return kEffectParamsProviderName;}

  // apply the parameters on effect
  virtual void UpdateParams(const FrameArgs& args) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(EffectParamsProvider);
};

typedef scoped_refptr<EffectParamsProvider> EffectParamsProviderPtr;
}  // namespace azer
