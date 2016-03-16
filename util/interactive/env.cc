#include "azer/util/interactive/env.h"

#include "base/files/file_path.h"
#include "azer/effect/effect.h"
#include "azer/effect/effectlib.h"
#include "azer/effect/effect_params_adapter.h"


namespace azer {
namespace {
::base::LazyInstance<InteractiveEnv> env_instance = LAZY_INSTANCE_INITIALIZER;
}
InteractiveEnv* InteractiveEnv::GetInstance() {
  return env_instance.Pointer();
}

InteractiveEnv::InteractiveEnv() {
  ::base::FilePath effectpath(FILE_PATH_LITERAL("out/dbg/shaderlib.hlsl.pak"));
  effectlib_.reset(new EffectLib);
  CHECK(effectlib_->Load(effectpath));

  DirLight dir;
  dir.diffuse = Vector4(0.5f, 0.5f, 0.4f, 1.0f);
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
}  // namespace azer
