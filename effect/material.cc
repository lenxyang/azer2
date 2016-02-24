#include "azer/effect/material.h"

#include "base/logging.h"
#include "base/files/file_path.h"
#include "azer/base/config_node.h"
#include "azer/base/image.h"
#include "azer/base/resource_loader.h"
#include "azer/util/images/image.h"
#include "azer/render/blending.h"
#include "azer/render/render_system.h"

namespace azer {
namespace {
TexturePtr LoadTexture(const ResPath& path, FileSystem* fs) {
  FileContents contents;
  if (!azer::LoadFileContents(path, &contents, fs)) {
    return TexturePtr();
  }

  const uint8* data = (uint8*)(&contents.front());
  ImageDataPtr imgdata(LoadDDSImageFromMemory(data, contents.size()));
  RenderSystem* rs = RenderSystem::Current();
  Texture::Options opt;
  opt.target = kBindTargetShaderResource;
  TexturePtr tex = rs->CreateTexture(opt, imgdata.get());
  return tex;
}
}  // namespace

DiffuseMapMaterialData::DiffuseMapMaterialData() 
    : ambient_scalar(0.1f),
      specular_scalar(1.0f), 
      alpha(1.0f) {
}

const char DiffuseMapMaterial::kEffectProviderName[] = "azer::DiffuseMapMaterial";;
DiffuseMapMaterial::DiffuseMapMaterial() {}
DiffuseMapMaterial::~DiffuseMapMaterial() {}
bool DiffuseMapMaterial::Init(const ConfigNode* node, ResourceLoadContext* ctx) {
  DiffuseMapMaterialData* data = mutable_data();
  if (!node->GetChildTextAsFloat("ambient_scalar", &data->ambient_scalar)) {
    return false;
  }
  if (!node->GetChildTextAsFloat("specular_scalar", &data->specular_scalar)) {
    return false;
  }
  if (!node->GetChildTextAsFloat("alpha", &data->alpha)) {
    return false;
  }

  std::string path;
  if (!node->GetChildText("diffusemap", &path)) {
    return false;
  }
  
  ResPath texpath(ResPath(::base::UTF8ToUTF16(path)));
  mutable_data()->diffusemap = LoadTexture(texpath, ctx->file_system);
  return (mutable_data()->diffusemap != NULL);
}
}  // namespace azer
