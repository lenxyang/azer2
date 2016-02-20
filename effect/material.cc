#include "azer/effect/material.h"

#include "azer/base/config_node.h"
#include "azer/resource/resource_loader.h"
#include "azer/render/blending.h"

namespace azer {
DiffuseMapMaterialData::DiffuseMapMaterialData() 
    : ambient_scalar(0.1f),
      specular_scalar(1.0f), 
      alpha(1.0f) {
}

DiffuseMapMaterial::DiffuseMapMaterial() {}
DiffuseMapMaterial::~DiffuseMapMaterial() {}
bool DiffuseMapMaterial::Init(const ConfigNode* node, ResourceLoadContext* ctx) {
  return false;
}
}  // namespace azer
