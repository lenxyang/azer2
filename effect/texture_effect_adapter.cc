#include "azer/effect/texture_effect_adapter.h"

#include "base/logging.h"
#include "azer/effect/texture_effect.h"
#include "azer/render/camera.h"
#include "azer/scene/scene_node.h"
#include "azer/scene/render_node.h"

namespace azer {
EffectAdapterKey TextureEffect_TextureMaterial_Adapter::key() const {
  return std::make_pair(typeid(TextureEffect).name(),
                        typeid(TextureMaterial).name());
}

void TextureEffect_TextureMaterial_Adapter::Apply(
    Effect* e, const EffectParamsProvider* params) const {
  CHECK(typeid(*e) == typeid(TextureEffect));
  CHECK(typeid(*params) == typeid(TextureMaterial));
  const TextureMaterial* provider = (const TextureMaterial*)params;
  TextureEffect* effect = dynamic_cast<TextureEffect*>(e);
  effect->SetMaterial(provider->data());
}
}  // namespace azer
