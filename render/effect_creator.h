#pragma once

#include "base/logging.h"
#include "azer/base/class_creator.h"
#include "azer/render/effect.h"

namespace azer {
template<class T>
class EffectAutoReg {
 public:
  EffectAutoReg() {
    ClassCreator::CreatorFunc func = base::Bind(T::CreateObject);
    CHECK(ClassCreator<Effect>::instance()->Register(T::kEffectName, func));
  }
};

EffectPtr CreateEffectByName(const std::string& name) {
  return EffectPtr(ClassCreator<Effect>::instance()->create(name));
}
}  // namespace azer

#define DECLARE_EFFECT_DYNCREATE(EFFECT_CLASS_NAME)             \
  static azer::EffectAutoReg<EFFECT_CLASS_NAME> effect_auto_reg_

#define IMPLEMENT_EFFECT_DYNCREATE(EFFECT_CLASS_NAME)                   \
  azer::EffectAutoReg<EFFECT_CLASS_NAME> EFFECT_CLASS_NAME::effect_auto_reg_
