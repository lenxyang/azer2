#include "azer/render/effect_params_provider.h"

#include "base/logging.h"

namespace azer {

const char EffectParamsAdapter::kEffectParamsAdapterName[] = 
    "azer::EffectParamsAdapter";
EffectParamsAdapter::EffectParamsAdapter() {
}

EffectParamsAdapter::~EffectParamsAdapter() {
}

const char* EffectParamsAdapter::GetAdapterName() const { 
  return kEffectParamsAdapterName;
}


const char EffectParamsProvider::kEffectParamsProviderName[] =
    "azer::EffectParamsProvider";
EffectParamsProvider::EffectParamsProvider() {
}

EffectParamsProvider::~EffectParamsProvider() {
}

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
    std::string effect_string, std::string provider_string) const {
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
    const std::vector<EffectParamsProviderPtr>* provider) 
    : providers_(provider),
      context_(context) {
}

EffectAdapterCache::~EffectAdapterCache() {
}

EffectAdapterCache::AdapterVector* EffectAdapterCache::GetAdapter(Effect* effect) {
  std::string effect_name = typeid(*effect).name();
  auto iter = cached_.find(effect_name);
  if (iter != cached_.end()) {
    return false;
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
    EffectParamsAdapter* adapter = (*adapter_vec)[i];
    const EffectParamsProviderPtr* provider = (*providers_)[i].get();
    adapter->Apply(effect, provider);
  }
}
}
