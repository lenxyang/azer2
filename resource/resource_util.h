#pragma once

#include <string>
#include "azer/base/export.h"
#include "azer/base/config_node.h"
#include "azer/base/resource_loader.h"

namespace azer {

VertexDescPtr LoadReferVertexDesc(const ConfigNode* node, ResourceLoadContext* ctx);
EffectPtr LoadReferEffect(const ConfigNode* node, ResourceLoadContext* ctx);
LightPtr LoadReferLight(const ConfigNode* node, ResourceLoadContext* ctx);
MeshPtr LoadReferMesh(const ConfigNode* node, ResourceLoadContext* ctx);
BaseMaterialPtr LoadReferMaterial(const ConfigNode* node, ResourceLoadContext* ctx);
VertexDescPtr LoadVertexDesc(const ResPath& path, ResourceLoadContext* ctx);
EffectPtr LoadEffect(const ResPath& path, ResourceLoadContext* ctx);

void InitDefaultLoader(ResourceLoader* loder);
}  // namespace azer
