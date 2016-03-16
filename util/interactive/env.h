#pragma once

#include <string>
#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "azer/base/export.h"
#include "azer/effect/light.h"
#include "azer/render/blending.h"

namespace azer {
class Effect;
class EffectLib;
class EffectAdapterContext;

class InteractiveEnv {
 public:
  static InteractiveEnv* GetInstance();

  Light* light() const { return light_.get();}
  Effect* GetEffect(const std::string& name);
  EffectAdapterContext* effect_context();
  Blending* blending() { return blending_.get();}
 private:
  InteractiveEnv();
  ~InteractiveEnv();

  LightPtr light_;
  scoped_ptr<EffectLib> effectlib_;
  BlendingPtr blending_;
  friend struct ::base::DefaultLazyInstanceTraits<InteractiveEnv>;
  DISALLOW_COPY_AND_ASSIGN(InteractiveEnv);
};
}  // namespace azer
