#pragma once

#include "azer/render/render.h"

namespace azer {
class EffectProvider : public EffectParamsProvider {
 public:
  EffectProvider(const DirLight* light) : light_(light) {}
  void Update(const Camera& camera) {
    world_ = std::move(holder_.GenWorldMatrix());
    pvw_ = std::move(camera.GetProjViewMatrix() * world_);
  }

  void Apply(Effect* e) {
    if (typeid(*e) == typeid(PVWEffect)) {
      PVWEffect* effect = dynamic_cast<PVWEffect*>(e);
      effect->SetWorld(world_);
      effect->SetPVW(pvw_);
    } else if (typeid(*e) == typeid(ColoredDiffuseEffect)) {
      ColoredDiffuseEffect* effect = dynamic_cast<ColoredDiffuseEffect*>(e);
      effect->SetWorld(world_);
      effect->SetPVW(pvw_);
      effect->SetDirLight(*light_);
      effect->SetColor(Vector4(1.0, 0.0, 0.0, 1.0));
    }
  }

  TransformHolder* GetTransformHolder() { return &holder_;}
  const TransformHolder* GetTransformHolder() const { return &holder_;}
 private:
  const DirLight* light_;
  Matrix4 world_;
  Matrix4 pvw_;
  TransformHolder holder_;
};

typedef scoped_ptr<EffectProvider> EffectProviderPtr;

}  // namespace azer
