#pragma once

#include "azer/render/render.h"

namespace azer {

class MaterialEffectProvider : public EffectParamsProvider {
 public:
  MaterialEffectProvider(){  }
  
  void UpdateParams(const FrameArgs& args) {}
};

class MaterialEffectAdapter : public EffectParamsAdapter {
 public:
  MaterialEffectAdapter() {}
  EffectAdapterKey key() const override {
    return std::make_pair(typeid(ColoredDiffuseEffect).name(), 
                          typeid(MaterialEffectProvider).name());
  }

  void Apply(Effect* e, const EffectParamsProvider* params) const override {
    CHECK(typeid(*e) == typeid(ColoredDiffuseEffect));
    CHECK(typeid(*params) == typeid(MaterialEffectProvider));
    MaterialEffectProvider* provider = (MaterialEffectProvider*)params;
    ColoredDiffuseEffect* effect = dynamic_cast<ColoredDiffuseEffect*>(e);
    effect->SetColor(Vector4(1.0, 0.0, 0.0, 1.0));
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(MaterialEffectAdapter);
};

class EnvironmentEffectProvider : public EffectParamsProvider {
 public:
  EnvironmentEffectProvider(const DirLight* light, Camera* camera) 
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

  const DirLight* light() const { return light_;}
  const Matrix4& world() const { return world_;}
  const Matrix4& pvw() const { return pvw_;}
 private:
  friend class PVWEffectAdapter;
  friend class ColoredEffectAdapter;
  const DirLight* light_;
  const Camera* camera_;
  Matrix4 world_;
  Matrix4 pvw_;
  TransformHolder holder_;
};

class EnvironmentEffectAdapter : public EffectParamsAdapter {
 public:
  EnvironmentEffectAdapter() {}
  EffectAdapterKey key() const override {
    return std::make_pair(typeid(ColoredDiffuseEffect).name(), 
                          typeid(EnvironmentEffectProvider).name());
  }

  void Apply(Effect* e, const EffectParamsProvider* params) const override {
    CHECK(typeid(*e) == typeid(ColoredDiffuseEffect));
    CHECK(typeid(*params) == typeid(EnvironmentEffectProvider));
    EnvironmentEffectProvider* provider = (EnvironmentEffectProvider*)params;
    ColoredDiffuseEffect* effect = dynamic_cast<ColoredDiffuseEffect*>(e);
    
    effect->SetWorld(provider->world());
    effect->SetPVW(provider->pvw());
    effect->SetDirLight(*(provider->light()));
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(EnvironmentEffectAdapter);
};


typedef scoped_refptr<EnvironmentEffectProvider> EnvironmentEffectProviderPtr;
}  // namespace azer
