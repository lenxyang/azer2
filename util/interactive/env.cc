#include "azer/util/interactive/env.h"

#include "base/files/file_path.h"
#include "azer/effect/effect.h"
#include "azer/effect/effectlib.h"
#include "azer/effect/effect_params_adapter.h"
#include "azer/util/images/image.h"
#include "azer/res/reslib.h"
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

  noncull_rasterizer_state_ = rs->CreateRasterizerState();
  noncull_rasterizer_state_->SetCullingMode(kCullNone);
}

InteractiveEnv::~InteractiveEnv() {}
Effect* InteractiveEnv::GetEffect(const std::string& name) {
  return ResLib::instance()->GetEffect(name);
}
EffectAdapterContext* InteractiveEnv::effect_context() {
  return ResLib::instance()->adapter_context();
}

Texture* InteractiveEnv::GetTexture(int32 id) {
  return ResLib::instance()->GetTexture(id);
}
}  // namespace azer
