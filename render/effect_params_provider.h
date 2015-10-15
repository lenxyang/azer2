#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {

class Effect;
class EffectAdapterContext;
class EffectAdapterCache;
class EffectParamsProvider;
class FrameArgs;

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
typedef std::vector<EffectParamsProviderPtr> EffectParamsProviderVector;

class AZER_EXPORT EffectParamsProviderContainer : 
      public ::base::RefCounted<EffectParamsProviderContainer> {
 public:
  EffectParamsProviderContainer();
  EffectParamsProviderContainer(EffectAdapterContext* context);
  virtual ~EffectParamsProviderContainer();

  void SetEffectAdapterContext(EffectAdapterContext* context);

  int32 provider_count() const;
  EffectParamsProviderPtr provider_at(int32 index);
  void AddProvider(EffectParamsProviderPtr provider);
  void RemoveProvider(EffectParamsProviderPtr provider);
  void ResetProvider();

  virtual void UpdateParams(const FrameArgs& args);
  void ApplyParams(Effect* effect);
 protected:
  void RebuildCache();
  EffectParamsProviderVector vector_;
  scoped_ptr<EffectAdapterCache> cached_;
  EffectAdapterContext* context_;
  DISALLOW_COPY_AND_ASSIGN(EffectParamsProviderContainer);
};

}  // namespace azer
