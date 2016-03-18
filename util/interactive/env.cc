#include "azer/util/interactive/env.h"

#include "base/files/file_path.h"
#include "azer/effect/effect.h"
#include "azer/effect/effectlib.h"
#include "azer/effect/effect_params_adapter.h"
#include "azer/util/images/image.h"
#include "azer/render/render_system.h"

namespace azer {
namespace {
::base::LazyInstance<InteractiveEnv> env_instance = LAZY_INSTANCE_INITIALIZER;
}
InteractiveEnv* InteractiveEnv::GetInstance() {
  return env_instance.Pointer();
}

InteractiveEnv::InteractiveEnv() {
  ::base::FilePath effectpath(FILE_PATH_LITERAL("out/dbg/azer.pak"));
  resource_pack_.reset(new ResourcePack);
  CHECK(resource_pack_->Load(effectpath));
  effectlib_.reset(new EffectLib(resource_pack_.get()));

  DirLight dir;
  dir.diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
  dir.ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
  dir.specular = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
  dir.directional = Vector4(1.0f, -1.0f, -1.0f, 0.0f);
  dir.factor = 1.0f;
  light_ = new Light(dir);

  azer::RenderSystem* rs = azer::RenderSystem::Current();
  Blending::Desc blend_desc;
  blend_desc.src = Blending::kSrcAlpha;
  blend_desc.dest = Blending::kSrcInvAlpha;
  blend_desc.oper = Blending::kAdd;
  blend_desc.src_alpha = Blending::kOne;
  blend_desc.dest_alpha = Blending::kZero;
  blend_desc.alpha_oper = Blending::kAdd;
  blend_desc.mask = Blending::kWriteColor;
  blending_ = rs->CreateBlending(blend_desc);
  blending_->EnableAlphaToConverage(true);
}

InteractiveEnv::~InteractiveEnv() {}
Effect* InteractiveEnv::GetEffect(const std::string& name) {
  return effectlib_->GetEffect(name);
}
EffectAdapterContext* InteractiveEnv::effect_context() {
  return effectlib_->adapter_context();
}

Texture* InteractiveEnv::GetTexture(int32 id) {
  auto iter = texture_.find(id);
  if (texture_.end() != iter) {
    return iter->second.get();
  }

  RenderSystem* rs = RenderSystem::Current();
  base::RefCountedStaticMemory* memory = resource_pack_->LoadDataResourceBytes(id);
  CHECK(memory);
  ImageDataPtr img = LoadDDSImageFromMemory(memory->front(), memory->size());
  Texture::Options opt;
  opt.target = kBindTargetShaderResource;
  TexturePtr tex = rs->CreateTexture(opt, img.get());
  texture_.insert(std::make_pair(id, tex));
  return tex;
}
}  // namespace azer
