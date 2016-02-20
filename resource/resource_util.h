#pragma once

#include <string>
#include "azer/base/config_node.h"
#include "azer/resource/resource_loader.h"

namespace azer {

ConfigNode* GetTypedReferNode(const std::string& type, const ConfigNode* parent);


VertexDescPtr LoadReferVertexDesc(const ConfigNode* node, 
                                        ResourceLoadContext* ctx);
EffectPtr LoadReferEffect(const ConfigNode* node,
                                ResourceLoadContext* ctx);
LightPtr LoadReferLight(const ConfigNode* node, ResourceLoadContext* ctx);
MeshPtr LoadReferMesh(const ConfigNode* node, ResourceLoadContext* ctx);
MaterialPtr LoadReferMaterial(const ConfigNode* node, ResourceLoadContext* ctx);
VariantResource LoadReferResource(const ConfigNode* node, ResourceLoadContext* ctx);

VertexDescPtr LoadVertexDesc(const ResPath& path, ResourceLoadContext* ctx);
EffectPtr LoadEffect(const ResPath& path, ResourceLoadContext* ctx);

VariantResource LoadResource(const ResPath& path, int type, ResourceLoadContext* );
VariantResource LoadResource(const ResPath& path, int type, ResourceLoader* loader);


bool Repath(const ResPath& path, ResPath* apath,
            ResourceLoadContext* ctx);
void InitDefaultLoader(ResourceLoader* loder);

}  // namespace azer
