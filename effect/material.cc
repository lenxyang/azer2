#include "azer/effect/material.h"

#include "base/logging.h"
#include "base/files/file_path.h"
#include "azer/base/config_node.h"
#include "azer/base/image.h"
#include "azer/base/image_data.h"
#include "azer/resource/resource_loader.h"
#include "azer/render/blending.h"
#include "samples/base/resource_util.h"

namespace azer {
namespace {
TexturePtr Load2DTexture(const ResPath& path, FileSystem* fs) {
  FileContents contents;
  if (!azer::LoadFileContents(path, &contents, fs)) {
    return TexturePtr();
  }

  const char* data = (char*)(&contents.front());
  ImageDataPtr imgdata(ImageData::Load2D(data, contents.size()));
  ImagePtr img(new Image(imgdata, kTex2D));
  RenderSystem* rs = RenderSystem::Current();
  Texture::Options opt;
  opt.target = kBindTargetShaderResource;
  opt.size = gfx::Size(img->width(), img->height());
  TexturePtr tex = rs->CreateTexture(opt, img.get());
  return tex;
}
}  // namespace

const char DiffuseMapMaterialData::kEffectProviderName[] = "azer::DiffuseMapMaterialData";;
DiffuseMapMaterialData::DiffuseMapMaterialData() 
    : ambient_scalar(0.1f),
      specular_scalar(1.0f), 
      alpha(1.0f) {
}

DiffuseMapMaterial::DiffuseMapMaterial() {}
DiffuseMapMaterial::~DiffuseMapMaterial() {}
bool DiffuseMapMaterial::Init(const ConfigNode* node, ResourceLoadContext* ctx) {
  if (!node->GetChildText("ambient_scalar", &mutable_data()->ambient_scalar)) {
    return false;
  }
  if (!node->GetChildText("specular_scalar", &mutable_data()->specular_scalar)) {
    return false;
  }
  if (!node->GetChildText("alpha", &mutable_data()->alpha)) {
    return false;
  }

  std::string path;
  if (!node->GetChildText("diffusemap", &path)) {
    return false;
  }
  
  ResPath texpath(ResPath(::base::UTF8ToUTF16(path)));
  mutable_data()->diffusemap = Load2DTexture(texpath, ctx->file_system());
  return (mutable_data()->diffusemap != NULL);
}
}  // namespace azer
