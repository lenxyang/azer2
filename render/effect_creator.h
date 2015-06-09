#pragma once

#include "base/logging.h"
#include "base/bind.h"
#include "base/callback.h"
#include "azer/base/class_creator.h"
#include "azer/render/effect.h"
#include "azer/render/effect_params_provider.h"

namespace azer {
template<class T>
class EffectAutoReg {
 public:
  EffectAutoReg() {
    ClassCreator<Effect>::CreatorFunc func = base::Bind(&T::CreateObject);
    CHECK(ClassCreator<Effect>::instance()->Register(T::kEffectName, func));
  }
};

template<class T>
class EffectProviderAutoReg {
 public:
  EffectProviderAutoReg() {
    ClassCreator<EffectParamsProvider>::CreatorFunc func = 
        base::Bind(&T::CreateObject);
    CHECK(ClassCreator<EffectParamsProvider>::instance()->Register(
        T::kEffectProviderName, func));
  }
};

// attention: the function must be a inline function
// because the ClassCreator<Effect>::instance() may be belong to another
// library(static library or dynamic library)
inline EffectPtr CreateEffectByName(const std::string& name) {
  return EffectPtr(ClassCreator<Effect>::instance()->create(name));
}

inline scoped_ptr<EffectParamsProvider> CreateEffectProviderByName(
    const std::string& name) {
  scoped_ptr<EffectParamsProvider> ptr(
      ClassCreator<EffectParamsProvider>::instance()->create(name));
  return ptr.Pass()
}
}  // namespace azer

#define DECLARE_EFFECT_DYNCREATE(EFFECT_CLASS_NAME)             \
  static azer::EffectAutoReg<EFFECT_CLASS_NAME> effect_auto_reg_

#define IMPLEMENT_EFFECT_DYNCREATE(EFFECT_CLASS_NAME)                   \
  azer::EffectAutoReg<EFFECT_CLASS_NAME> EFFECT_CLASS_NAME::effect_auto_reg_

#define DECLARE_EFFECT_PROVIDER_DYNCREATE(EFFECT_PROVIDER_CLASS_NAME)   \
  static azer::EffectAutoReg<EFFECT_PROVIDER_CLASS_NAME>                \
  effect_provider_auto_reg_

#define IMPLEMENT_EFFECT_PROVIDER_DYNCREATE(EFFECT_PROVIDER_CLASS_NAME) \
  azer::EffectAutoReg<EFFECT_PROVIDER_CLASS_NAME> \
  EFFECT_PROVIDER_CLASS_NAME::effect_provider_auto_reg_
