#include "azer/effect/effect_params_adapter.h"

#include "base/logging.h"
#include "azer/effect/effect_params_provider.h"
#include "azer/render/frame_args.h"

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


// class EffectParamsProvider
const char EffectParamsProvider::kEffectParamsProviderName[] =
    "azer::EffectParamsProvider";
EffectParamsProvider::EffectParamsProvider() : args_(NULL) {}
EffectParamsProvider::~EffectParamsProvider() {}


// class EffectParamsProviderContainer
EffectParamsProviderContainer::EffectParamsProviderContainer()
    : context_(NULL) {
}

EffectParamsProviderContainer::EffectParamsProviderContainer
(EffectAdapterContext* context)
    : context_(context) {
}

EffectParamsProviderContainer::~EffectParamsProviderContainer() {
}

void EffectParamsProviderContainer::RebuildCache() {
  if (context_) {
    cached_.reset(new EffectAdapterCache(context_, &providers_));
  } else {
    cached_.reset();
  }
}

void EffectParamsProviderContainer::SetEffectAdapterContext(
    EffectAdapterContext* context) {
  context_ = context;
  RebuildCache();
}

int32_t EffectParamsProviderContainer::provider_count() const {
  return static_cast<int32_t>(providers_.size());
}

EffectParamsProviderPtr EffectParamsProviderContainer::provider_at(int32_t index) {
  return providers_.at(index);
}

void EffectParamsProviderContainer::AddProvider(EffectParamsProviderPtr provider) {
  providers_.push_back(provider);
  RebuildCache();
}

void EffectParamsProviderContainer::RemoveProvider(EffectParamsProviderPtr provider) {
  int index = 0;
  for (auto iter = providers_.begin(); iter != providers_.end(); ++iter, ++index) {
    if (iter->get() == provider) {
      providers_.erase(iter);
      break;
    }
  }

  RebuildCache();
}

void EffectParamsProviderContainer::ResetProvider() {
  providers_.clear();
  RebuildCache();
}

void EffectParamsProviderContainer::ApplyParams(Effect* effect) {
  if (providers_.size() > 0u) {
    DCHECK(cached_.get());
    cached_->ApplyParams(effect);
  }
}

}  // namespace azer
