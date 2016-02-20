#pragma once

#include "azer/render/mesh.h"
#include "azer/effect/light.h"
#include "azer/effect/base_material.h"
#include "azer/render/vertex_buffer.h"
#include "azer/scene/scene_node.h"

namespace azer {
enum {
  kResTypeNone,
  kResTypeMesh,
  kResTypeMaterial,
  kResTypeEffect,
  kResTypeLight,
  kResTypeScene,
  kResTypeVertexDesc,
};

struct VariantResource {
  MeshPtr mesh;
  EffectPtr effect;
  VertexDescPtr vertex_desc;
  BaseMaterialPtr material;
  LightPtr light;
  SceneNodePtr scene;
  int32 type;
  int32 retcode;

  VariantResource() : type(kResTypeNone), retcode(-1) {}
};

int32 GetTypeFromString(const std::string& str);
}  // namespace azer
