#include "azer/render/effect_params_adapter.h"

#include "azer/render/effect.h"
#include "azer/render/effect_params_provider.h"

namespace azer {
// EffectAdapterContext
EffectAdapterContext::EffectAdapterContext() {
}

EffectAdapterContext::~EffectAdapterContext() {
}

void EffectAdapterContext::RegisteAdapter(EffectParamsAdapter* adapter) {
  const EffectAdapterKey& key = adapter->key();
  auto iter = dict_.find(key);
  DCHECK(iter == dict_.end());
  dict_.insert(std::make_pair(key, adapter));
}

const EffectParamsAdapter* EffectAdapterContext::LookupAdapter(
    const std::string& effect_string, const std::string& provider_string) const {
  EffectAdapterKey key = std::make_pair(effect_string, provider_string);
  auto iter = dict_.find(key);
  if (iter != dict_.end()) {
    return iter->second;
  } else {
    return NULL;
  }
}

// class EffectAdapterCache
EffectAdapterCache::EffectAdapterCache(
    EffectAdapterContext* context, 
    const EffectParamsProviderVector* provider)
    : providers_(provider),
      context_(context) {
}

EffectAdapterCache::~EffectAdapterCache() {
}

EffectAdapterCache::AdapterVector* EffectAdapterCache::GetAdapter(Effect* effect) {
  std::string effect_name = typeid(*effect).name();
  auto iter = cached_.find(effect_name);
  if (iter != cached_.end()) {
    return iter->second.get();
  } else {
    scoped_ptr<AdapterVector> adapters(new AdapterVector);
    for (auto iter = providers_->begin(); iter != providers_->end(); ++iter) {
      std::string provider_name = typeid(*((*iter).get())).name();
      adapters->push_back(context_->LookupAdapter(effect_name, provider_name));
      
    }
    cached_.insert(std::make_pair(effect_name, adapters.Pass()));
    return GetAdapter(effect);
  }
}

void EffectAdapterCache::ApplyParams(Effect* effect) {
  AdapterVector* adapter_vec = GetAdapter(effect);
  for (uint32 i = 0; i < adapter_vec->size(); ++i) {
    const EffectParamsAdapter* adapter = (*adapter_vec)[i];
    const EffectParamsProvider* provider = (*providers_)[i].get();
    adapter->Apply(effect, provider);
  }
}

}  // namespace azer
