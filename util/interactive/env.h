#pragma once

#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "azer/base/export.h"

namespace azer {
class EffectLib;
class EffectAdapterContext;

class InteractiveEnv {
 public:
  static InteractiveEnv* GetInstance();

  EffectAdapterContext* effect_context();
 private:
  InteractiveEnv();
  ~InteractiveEnv();

  scoped_ptr<EffectLib> effectlib_;
  DISALLOW_COPY_AND_ASSIGN(InteractiveEnv);
};
}  // namespace azer
