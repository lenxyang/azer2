#include "azer/render/effect_params_adapter.h"

#include "base/logging.h"
#include "azer/render/frame_args.h"
#include "azer/render/effect_params_provider.h"

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
EffectParamsProvider::EffectParamsProvider() {}
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
    cached_.reset(new EffectAdapterCache(context_, &vector_));
  } else {
    cached_.reset();
  }
}

void EffectParamsProviderContainer::SetEffectAdapterContext(
    EffectAdapterContext* context) {
  context_ = context;
  RebuildCache();
}

int32 EffectParamsProviderContainer::provider_count() const {
  return static_cast<int32>(vector_.size());
}

EffectParamsProviderPtr EffectParamsProviderContainer::provider_at(int32 index) {
  return vector_.at(index);
}

void EffectParamsProviderContainer::AddProvider(EffectParamsProviderPtr provider) {
  vector_.push_back(provider);
  RebuildCache();
}

void EffectParamsProviderContainer::RemoveProvider(EffectParamsProviderPtr provider) {
  int index = 0;
  for (auto iter = vector_.begin(); iter != vector_.end(); ++iter, ++index) {
    if (iter->get() == provider) {
      vector_.erase(iter);
      break;
    }
  }

  RebuildCache();
}

void EffectParamsProviderContainer::ResetProvider() {
  vector_.clear();
  RebuildCache();
}

void EffectParamsProviderContainer::ApplyParams(Effect* effect) {
  if (vector_.size() > 0u) {
    DCHECK(cached_.get());
    cached_->ApplyParams(effect);
  }
}

}
