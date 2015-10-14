#pragma once

#include <typeindex>
#include "azer/render/render.h"
#include "azer/render/util.h"

namespace azer {

class EffectProvider : public EffectParamsProvider {
 public:
  EffectProvider(const DirLight* light, Camera* camera) 
      : light_(light),
        camera_(camera) {
  }
  
  void UpdateParams(const FrameArgs& args) {
    Radians rad((3.14f) * (args.delta().InSecondsF()) * 0.2f);
    holder_.rotate(Vector3(0.0f, 1.0f, 0.0f), rad);
    world_ = std::move(holder_.GenWorldMatrix());
    pvw_ = std::move(camera_->GetProjViewMatrix() * world_);
  }

  TransformHolder* GetTransformHolder() { return &holder_;}
  const TransformHolder* GetTransformHolder() const { return &holder_;}
 private:
  friend class PVWEffectAdapter;
  friend class ColoredEffectAdapter;
  const DirLight* light_;
  const Camera* camera_;
  Matrix4 world_;
  Matrix4 pvw_;
  TransformHolder holder_;
};

class ColoredEffectAdapter : public EffectParamsAdapter {
 public:
  ColoredEffectAdapter() {}
  EffectAdapterKey key() const override {
    return std::make_pair(typeid(ColoredDiffuseEffect).name(), 
                          typeid(EffectProvider).name());
  }

  void Apply(Effect* e, const EffectParamsProvider* params) const override {
    CHECK(typeid(*e) == typeid(ColoredDiffuseEffect));
    CHECK(typeid(*params) == typeid(EffectProvider));
    EffectProvider* provider = (EffectProvider*)params;
    ColoredDiffuseEffect* effect = dynamic_cast<ColoredDiffuseEffect*>(e);
    
    effect->SetWorld(provider->world_);
    effect->SetPVW(provider->pvw_);
    effect->SetDirLight(*(provider->light_));
    effect->SetColor(Vector4(1.0, 0.0, 0.0, 1.0));
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(ColoredEffectAdapter);
};

typedef scoped_refptr<EffectProvider> EffectProviderPtr;

}  // namespace azer
