#pragma once

#include "azer/render/render.h"

namespace azer {

class EffectProvider : public EffectParamsProvider {
 public:
  EffectProvider(const DirLight* light, Camera* camera) 
      : light_(light),
        camera_(camera) {
  }
  
  void UpdateParams(const FrameArgs& args) {
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

class PVWEffectAdapter : public EffectParamsAdapter {
 public:
  PVWEffectAdapter() {}
  void Apply(Effect* e, EffectParamsProvider* params) override {
    CHECK(typeid(*e) == typeid(PVWEffect));
    CHECK(typeid(*params) == typeid(EffectProvider));
    PVWEffect* effect = dynamic_cast<PVWEffect*>(e);
    EffectProvider* provider = (EffectProvider*)params;
    effect->SetWorld(provider->world_);
    effect->SetPVW(provider->pvw_);
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(PVWEffectAdapter);
};

class ColoredEffectAdapter : public EffectParamsAdapter {
 public:
  ColoredEffectAdapter() {}
  void Apply(Effect* e, EffectParamsProvider* params) override {
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
