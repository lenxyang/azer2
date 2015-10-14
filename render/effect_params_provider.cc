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

void EffectAdapterContext::RegisteAdapter(std::type_index effect_type_index,
                                          std::type_index provider_type_index,
                                          EffectParamsAdapter* adapter) {
  EffectAdapterKey key = std::make_pair(effect_type_index, provider_type_index);
  auto iter = dict_.find(key);
  DCHECK(iter == dict_.end());
  dict_.insert(std::make_pair(key, adapter));
}

const EffectParamsAdapter* EffectAdapterContext::LookupAdapter(
    std::type_index effect_type_index, std::type_index provider_type_index) const {
  EffectAdapterKey key = std::make_pair(effect_type_index, provider_type_index);
  auto iter = dict_.find(key);
  if (iter != dict_.end()) {
    return iter->second;
  } else {
    return NULL;
  }
}
}
