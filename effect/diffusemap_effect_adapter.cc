#include "azer/effect/diffusemap_effect_adapter.h"

#include "base/logging.h"
#include "azer/effect/diffusemap_effect.h"
#include "azer/render/camera.h"
#include "azer/scene/scene_node.h"
#include "azer/scene/render_node.h"

namespace azer {
EffectAdapterKey DiffuseMapEffect_DiffuseMapMaterial_Adapter::key() const {
  return std::make_pair(typeid(DiffuseMapEffect).name(),
                        typeid(DiffuseMapMaterial).name());
}

void DiffuseMapEffect_DiffuseMapMaterial_Adapter::Apply(
    Effect* e, const EffectParamsProvider* params) const {
  CHECK(typeid(*e) == typeid(DiffuseMapEffect));
  CHECK(typeid(*params) == typeid(DiffuseMapMaterial));
  const DiffuseMapMaterial* provider = (const DiffuseMapMaterial*)params;
  DiffuseMapEffect* effect = dynamic_cast<DiffuseMapEffect*>(e);
  effect->SetMaterial(provider->data());
}

EffectAdapterKey DiffuseMapEffect_RenderNode_Adapter::key() const {
  return std::make_pair(typeid(DiffuseMapEffect).name(),
                        typeid(RenderNode).name());
}

void DiffuseMapEffect_RenderNode_Adapter::Apply(
    Effect* e, const EffectParamsProvider* params) const {
  CHECK(typeid(*e) == typeid(DiffuseMapEffect));
  CHECK(typeid(*params) == typeid(RenderNode));
  const RenderNode* provider = (const RenderNode*)params;
  DiffuseMapEffect* effect = dynamic_cast<DiffuseMapEffect*>(e);
  effect->SetWorld(provider->GetWorld());
  effect->SetPV(provider->camera()->GetProjViewMatrix());
  effect->SetCameraPos(Vector4(provider->camera()->position(), 1.0));
}

}  // namespace azer
