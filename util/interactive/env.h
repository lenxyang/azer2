#pragma once

#include "base/lazy_instance.h"
#include "azer/base/export.h"

namespace azer {
class InteractiveEnv {
 public:
  static InteractiveEnv* GetInstance();

  azer::EffectAdapterContext* effect_context();
 private:
  InteractiveEnv();

  scoped_ptr<azer::EffectLib> effectlib_;
  DISALLOW_COPY_AND_ASSIGN(InteractiveEnv);
};
}  // namespace azer
