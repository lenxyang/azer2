#include "azer/resource/resource_util.h"

#include "base/logging.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/base/file_system.h"
#include "azer/base/resource_loader.h"
#include "azer/render/blending.h"
#include "azer/resource/xml_util.h"
#include "azer/resource/effect_loader.h"
#include "azer/resource/light_loader.h"
#include "azer/resource/material_loader.h"
#include "azer/resource/mesh_loader.h"
#include "azer/resource/scene_loader.h"
#include "azer/resource/vertex_desc_loader.h"

namespace azer {

void InitDefaultLoader(ResourceLoader* loader) {
  loader->RegisterSpecialLoader(new EffectLoader);
  loader->RegisterSpecialLoader(new MeshLoader);
  loader->RegisterSpecialLoader(new VertexDescLoader);
  loader->RegisterSpecialLoader(new LightLoader);
  loader->RegisterSpecialLoader(new SceneLoader);
  loader->RegisterSpecialLoader(new MaterialLoader);
}

MeshPtr LoadReferMesh(const ConfigNode* node, ResourceLoadContext* ctx) {
  VariantResource ret = LoadReferResource(node, ctx);
  if (ret.type == kResTypeMesh) {
    return ret.mesh;
  } else {
    return MeshPtr();
  }
}

LightPtr LoadReferLight(const ConfigNode* node, ResourceLoadContext* ctx) {
  VariantResource ret = LoadReferResource(node, ctx);
  if (ret.type == kResTypeLight) {
    return ret.light;
  } else {
    return LightPtr();
  }
}

EffectPtr LoadReferEffect(const ConfigNode* node, ResourceLoadContext* ctx) {
  VariantResource ret = LoadReferResource(node, ctx);
  if (ret.type == kResTypeEffect) {
    return ret.effect;
  } else {
    return EffectPtr();
  }
}

VertexDescPtr LoadReferVertexDesc(const ConfigNode* node, 
                                  ResourceLoadContext* ctx) {
  VariantResource ret = LoadReferResource(node, ctx);
  if (ret.type == kResTypeVertexDesc) {
    return ret.vertex_desc;
  } else {
    return VertexDescPtr();
  }
}

BaseMaterialPtr LoadReferMaterial(const ConfigNode* node, ResourceLoadContext* ctx) {
  VariantResource ret = LoadReferResource(node, ctx);
  if (ret.type == kResTypeMaterial) {
    return ret.material;
  } else {
    return BaseMaterialPtr();
  }
}

VertexDescPtr LoadVertexDesc(const ResPath& path, ResourceLoadContext* ctx) {
  VariantResource res = LoadResource(path, kResTypeVertexDesc, ctx);
  return res.vertex_desc;
}

EffectPtr LoadEffect(const ResPath& path, ResourceLoadContext* ctx) {
  VariantResource res = LoadResource(path, kResTypeEffect, ctx);
  return res.effect;
}
}  // namespace azer
