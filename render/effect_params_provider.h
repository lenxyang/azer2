#pragma once

#include <map>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {

class Effect;
class EffectParamsProvider;
class FrameArgs;

typedef std::pair<std::string, std::string> EffectAdapterKey;

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

  void RegisteAdapter(EffectParamsAdapter* adapter);
                      
  const EffectParamsAdapter* LookupAdapter(
      std::string effect_string, std::string provider_string) const;
 private:
  std::map<EffectAdapterKey,  const EffectParamsAdapter*> dict_;
  DISALLOW_COPY_AND_ASSIGN(EffectAdapterContext);
};

typedef scoped_refptr<EffectParamsProvider> EffectParamsProviderPtr;

class AZER_EXPORT EffectAdapterCache {
 public:
  EffectAdapterCache(EffectAdapterContext* context,
                     const std::vector<EffectParamsProviderPtr>* provider);
  ~EffectAdapterCache();

  void ApplyParams(Effect* effect);
 private:
  typedef std::vector<EffectParamsAdapter*> AdapterVector;
  AdapterVector* GetAdapter(Effect* effect);

  AdapterVector adapter_;
  std::map<std::string, scoped_ptr<AdapterVector> > cached_;
  const std::vector<EffectParamsProviderPtr>* providers_;
  EffectAdapterContext* context_;
  DISALLOW_COPY_AND_ASSIGN(EffectAdapterCache);
};
}  // namespace azer
