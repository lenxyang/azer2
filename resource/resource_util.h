#pragma once

#include <string>
#include "azer/base/config_node.h"
#include "azer/resource/resource_loader.h"

namespace azer {

AZER_EXPORT ConfigNode* GetTypedReferNode(const std::string& type, 
                                          const ConfigNode* parent);
AZER_EXPORT VertexDescPtr LoadReferVertexDesc(const ConfigNode* node, 
                                              ResourceLoadContext* ctx);
AZER_EXPORT EffectPtr LoadReferEffect(const ConfigNode* node,
                                      ResourceLoadContext* ctx);
AZER_EXPORT LightPtr LoadReferLight(const ConfigNode* node, 
                                    ResourceLoadContext* ctx);
AZER_EXPORT MeshPtr LoadReferMesh(const ConfigNode* node, ResourceLoadContext* ctx);
AZER_EXPORT BaseMaterialPtr LoadReferMaterial(const ConfigNode* node, 
                                              ResourceLoadContext* ctx);
AZER_EXPORT VariantResource LoadReferResource(const ConfigNode* node, 
                                              ResourceLoadContext* ctx);

AZER_EXPORT VertexDescPtr LoadVertexDesc(const ResPath& path, 
                                         ResourceLoadContext* ctx);
AZER_EXPORT EffectPtr LoadEffect(const ResPath& path, ResourceLoadContext* ctx);

AZER_EXPORT VariantResource LoadResource(const ResPath& path, int type, 
                                         ResourceLoadContext* );
AZER_EXPORT VariantResource LoadResource(const ResPath& path, int type, 
                                         ResourceLoader* loader);


AZER_EXPORT bool Repath(const ResPath& path, ResPath* apath, 
                        ResourceLoadContext* ctx);
AZER_EXPORT void InitDefaultLoader(ResourceLoader* loder);
}  // namespace azer
