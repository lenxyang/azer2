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
  virtual const char* GetProviderName() const { return kEffectParamsProviderName;}
 private:
  const FrameArgs* args_;
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

  EffectAdapterContext* adapter_context() { return context_;}
  void SetEffectAdapterContext(EffectAdapterContext* context);

  int32 provider_count() const;
  EffectParamsProviderPtr provider_at(int32 index);
  void AddProvider(EffectParamsProviderPtr provider);
  void RemoveProvider(EffectParamsProviderPtr provider);
  void ResetProvider();
  void ApplyParams(Effect* effect);
 protected:
  void RebuildCache();
  EffectParamsProviderVector providers_;
  scoped_ptr<EffectAdapterCache> cached_;
  EffectAdapterContext* context_;
  DISALLOW_COPY_AND_ASSIGN(EffectParamsProviderContainer);
};

}  // namespace azer
