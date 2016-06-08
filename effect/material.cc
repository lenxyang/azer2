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

  const uint8_t* data = (uint8_t*)(&contents.front());
  int len = static_cast<int>(contents.size());
  ImageDataPtr imgdata(LoadDDSImageFromMemory(data, len));
  RenderSystem* rs = RenderSystem::Current();
  Texture::Options opt;
  opt.target = kBindTargetShaderResource;
  TexturePtr tex = rs->CreateTexture(opt, imgdata.get());
  return tex;
}
}  // namespace

ColorMaterialData::ColorMaterialData() 
    : specular_power(1.0f), 
      alpha(1.0f),
      padding1(0.0f),
      padding2(0.0f) {
}

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

  RenderSystem* rs = RenderSystem::Current();
  ResPath texpath(ResPath(::base::UTF8ToUTF16(path)));
  TexturePtr tex = LoadTexture(texpath, ctx->file_system);
  mutable_data()->diffusemap = rs->CreateTextureView(
      TextureView::Options(), tex.get());
  return (mutable_data()->diffusemap != NULL);
}

TextureMaterialData::TextureMaterialData()
    : ambient_scalar(0.1f),
      specular_scalar(1.0f), 
      alpha(1.0f) {
}
const char TextureMaterial::kEffectProviderName[] = "azer::TextureMaterial";
TextureMaterial::TextureMaterial() {}
TextureMaterial::~TextureMaterial() {}
bool TextureMaterial::Init(const ConfigNode* node, ResourceLoadContext* ctx) {
  return false;
}
}  // namespace azer
