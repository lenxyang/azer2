#pragma once

#include <string>
#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "azer/base/export.h"

namespace azer {
class Effect;
class EffectLib;
class EffectAdapterContext;

class InteractiveEnv {
 public:
  static InteractiveEnv* GetInstance();

  Effect* GetEffect(const std::string& name);
  EffectAdapterContext* effect_context();
 private:
  InteractiveEnv();
  ~InteractiveEnv();

  scoped_ptr<EffectLib> effectlib_;
  friend struct ::base::DefaultLazyInstanceTraits<InteractiveEnv>;
  DISALLOW_COPY_AND_ASSIGN(InteractiveEnv);
};
}  // namespace azer
