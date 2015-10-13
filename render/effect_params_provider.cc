#include "azer/render/effect_params_provider.h"

namespace azer {

const char EffectParamsAdapter::kEffectParamsAdapterName[] = 
    "azer::EffectParamsAdapter";
EffectParamsAdapter::EffectParamsAdapter() {
}

EffectParamsAdapter::~EffectParamsAdapter() {
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

void EffectAdapterContext::RegisteAdapter(std::type_index effect_type_index,
                                          std::type_index provider_type_index,
                                          EffectParamsAdapter* adapter) {
  DictKey key = std::make_pair(effect_type_index, provider_type_index);
  auto iter = dict_.find(key);
  DCHECK(iter == dict_.end());
  dict_.insert(std::make_pair(key, adapter));
}

const EffectParamsAdapter* EffectAdapterContext::LookupAdapter(
    std::type_index effect_type_index, std::type_index provider_type_index) const {
  DictKey key = std::make_pair(effect_type_index, provider_type_index);
  auto iter = dict_.find(key);
  if (iter != dict_.end()) {
    return iter->second;
  } else {
    return NULL;
  }
}
}
