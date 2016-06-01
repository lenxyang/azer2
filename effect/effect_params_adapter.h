#pragma once

#include <map>
#include <vector>

#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {
class Effect;
class EffectAdapterContext;
class EffectParamsProvider;

typedef scoped_refptr<EffectParamsProvider> EffectParamsProviderPtr;
typedef std::vector<EffectParamsProviderPtr> EffectParamsProviderVector;

typedef std::pair<std::string, std::string> EffectAdapterKey;

class AZER_EXPORT EffectParamsAdapter {
 public:
  static const char kEffectParamsAdapterName[];
  EffectParamsAdapter();
  virtual ~EffectParamsAdapter();

  virtual const char* GetAdapterName() const;
  virtual EffectAdapterKey key() const = 0;
  virtual void Apply(Effect* effect, const EffectParamsProvider* params) const = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(EffectParamsAdapter);
};

class AZER_EXPORT EffectAdapterCache {
 public:
  EffectAdapterCache(EffectAdapterContext* context,
                     const EffectParamsProviderVector* provider);
  ~EffectAdapterCache();

  EffectAdapterContext* context() { return context_;}
  void ApplyParams(Effect* effect);
 private:
  typedef std::vector<const EffectParamsAdapter*> AdapterVector;
  AdapterVector* GetAdapter(Effect* effect);

  AdapterVector adapter_;
  std::map<std::string, AdapterVector> cached_;
  const EffectParamsProviderVector* providers_;
  EffectAdapterContext* context_;
  DISALLOW_COPY_AND_ASSIGN(EffectAdapterCache);
};

class AZER_EXPORT EffectAdapterContext {
 public:
  EffectAdapterContext();
  ~EffectAdapterContext();

  void RegisteAdapter(EffectParamsAdapter* adapter);
  const EffectParamsAdapter* LookupAdapter(const EffectAdapterKey& key) const;
  const EffectParamsAdapter* LookupAdapter(const std::string& effect_string,
                                           const std::string& provider_string) const;
 private:
  std::map<EffectAdapterKey,  const EffectParamsAdapter*> dict_;
  DISALLOW_COPY_AND_ASSIGN(EffectAdapterContext);
};
}  // namespace azer
