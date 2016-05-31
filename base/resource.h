#pragma once

#include <string>
#include <memory>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/values.h"
#include "azer/base/export.h"
#include "azer/effect/base_material.h"
#include "azer/effect/effect.h"
#include "azer/effect/light.h"
#include "azer/render/blending.h"
#include "azer/render/vertex_buffer.h"
#include "azer/scene/scene_node.h"
#include "azer/render/mesh.h"

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

struct AZER_EXPORT VariantResource {
  MeshPtr mesh;
  EffectPtr effect;
  VertexDescPtr vertex_desc;
  BaseMaterialPtr material;
  LightPtr light;
  SceneNodePtr scene;
  int32_t type;
  int32_t retcode;

  VariantResource();
};

AZER_EXPORT int32_t GetTypeFromString(const std::string& str);
}  // namespace azer
