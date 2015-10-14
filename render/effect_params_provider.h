#pragma once

#include <map>
#include <typeinfo>
#include <typeindex>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {

class Effect;
class EffectParamsProvider;
class FrameArgs;

typedef std::pair<std::type_index, std::type_index> EffectAdapterKey;

class AZER_EXPORT EffectParamsAdapter {
 public:
  static const char kEffectParamsAdapterName[];
  EffectParamsAdapter();
  virtual ~EffectParamsAdapter();

  virtual const char* GetAdapterName() const;
  virtual EffectAdapterKey key() const = 0;
  virtual void Apply(Effect* effect, EffectParamsProvider* params) const = 0;
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

class AZER_EXPORT EffectAdapterContext {
 public:
  EffectAdapterContext();
  ~EffectAdapterContext();

  void RegisteAdapter(std::type_index effect_type_index,
                      std::type_index provider_type_index,
                      EffectParamsAdapter* adapter);
                      
  const EffectParamsAdapter* LookupAdapter(
      std::type_index effect_type_index, std::type_index provider_type_index) const;
 private:
  std::map<EffectAdapterKey,  const EffectParamsAdapter*> dict_;
  DISALLOW_COPY_AND_ASSIGN(EffectAdapterContext);
};

typedef scoped_refptr<EffectParamsProvider> EffectParamsProviderPtr;
}  // namespace azer
