#pragma once

#include <string>
#include <map>

#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "azer/base/export.h"
#include "azer/effect/light.h"
#include "azer/render/blending.h"
#include "azer/render/rasterizer_state.h"
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
  Blending* a2c_blending() { return a2c_blending_.get();}
  Texture* GetTexture(int32_t id);
  ResourcePack* resource_pack();
  RasterizerState* wireframe_rasterizer_state();
  RasterizerState* noncull_rasterizer_state();
  DepthStencilState* depth_disabled_state();
 private:
  InteractiveEnv();
  ~InteractiveEnv();

  LightPtr light_;
  BlendingPtr blending_;
  BlendingPtr a2c_blending_;
  RasterizerStatePtr noncull_rasterizer_state_;
  RasterizerStatePtr wireframe_rasterizer_state_;
  DepthStencilStatePtr depth_disabled_state_;
  friend struct ::base::DefaultLazyInstanceTraits<InteractiveEnv>;
  DISALLOW_COPY_AND_ASSIGN(InteractiveEnv);
};
}  // namespace azer
