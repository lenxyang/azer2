#pragma once

#include <string>
#include <map>

#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "azer/base/export.h"
#include "azer/effect/light.h"
#include "azer/render/blending.h"
#include "azer/render/texture.h"

namespace azer {
class Effect;
class EffectLib;
class EffectAdapterContext;
class ResourcePack;

class InteractiveEnv {
 public:
  static InteractiveEnv* GetInstance();

  Light* light() const { return light_.get();}
  Effect* GetEffect(const std::string& name);
  EffectAdapterContext* effect_context();
  Blending* blending() { return blending_.get();}
  Texture* GetTexture(int32 id);
  ResourcePack* resource_pack() { return resource_pack_.get();}
 private:
  InteractiveEnv();
  ~InteractiveEnv();

  LightPtr light_;
  scoped_ptr<EffectLib> effectlib_;
  BlendingPtr blending_;
  std::map<int, TexturePtr> texture_;
  scoped_ptr<ResourcePack> resource_pack_;
  friend struct ::base::DefaultLazyInstanceTraits<InteractiveEnv>;
  DISALLOW_COPY_AND_ASSIGN(InteractiveEnv);
};
}  // namespace azer
