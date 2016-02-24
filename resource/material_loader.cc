#include "azer/resource/material_loader.h"

#include "base/strings/utf_string_conversions.h"
#include "azer/base/config_node.h"
#include "azer/effect/effect_creator.h"
#include "azer/effect/material.h"
#include "azer/render/blending.h"

namespace azer {

const char MaterialLoader::kSpecialLoaderName[] = "azer::MaterialLoader";
MaterialLoader::MaterialLoader() {
}

MaterialLoader::~MaterialLoader() {
}

const char* MaterialLoader::GetLoaderName() const {
  return kSpecialLoaderName;
}

VariantResource MaterialLoader::Load(const ConfigNode* node,
                                     ResourceLoadContext* ctx) {
  EffectParamsProviderPtr provider = 
      CreateEffectProviderByName(node->GetAttr("provider_name"));
  BaseMaterial* mtrl = dynamic_cast<BaseMaterial*>(provider.get());
  if (!mtrl || !mtrl->Init(node, ctx)) {
    return VariantResource();
  }

  VariantResource ret;
  ret.type = kResTypeMaterial;
  ret.retcode = 0;
  ret.material = mtrl;
  return ret;
}

bool MaterialLoader::CouldLoad(ConfigNode* node) const {
  return node->tagname() == "material";
}
}  // namespace azer
