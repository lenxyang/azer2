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
  Blending::BlendDesc blend_desc;
  blend_desc.desc[0].src = Blending::kSrcAlpha;
  blend_desc.desc[0].dest = Blending::kSrcInvAlpha;
  blend_desc.desc[0].oper = Blending::kAdd;
  blend_desc.desc[0].src_alpha = Blending::kOne;
  blend_desc.desc[0].dest_alpha = Blending::kZero;
  blend_desc.desc[0].alpha_oper = Blending::kAdd;
  blend_desc.desc[0].mask = Blending::kWriteColor;
  blend_desc.desc_count = 1;
  blend_desc.alpha_to_converage = false;
  blending_ = rs->CreateBlending(blend_desc);
  blend_desc.alpha_to_converage = true;
  a2c_blending_ = rs->CreateBlending(blend_desc);

  noncull_rasterizer_state_ = rs->CreateRasterizerState();
  noncull_rasterizer_state_->SetCullingMode(kCullNone);

  wireframe_rasterizer_state_ = rs->CreateRasterizerState();
  wireframe_rasterizer_state_->SetFillMode(kWireFrame);

  depth_disabled_state_ = rs->CreateDepthStencilState();
  depth_disabled_state_->EnableDepthTest(false);
}

InteractiveEnv::~InteractiveEnv() {}
Effect* InteractiveEnv::GetEffect(const std::string& name) {
  return ResLib::instance()->GetEffect(name);
}
EffectAdapterContext* InteractiveEnv::effect_context() {
  return ResLib::instance()->effect_context();
}

Texture* InteractiveEnv::GetTexture(int32 id) {
  return ResLib::instance()->GetTexture(id);
}

RasterizerState* InteractiveEnv::wireframe_rasterizer_state() { 
  return wireframe_rasterizer_state_;
}

RasterizerState* InteractiveEnv::noncull_rasterizer_state() { 
  return noncull_rasterizer_state_;
}

DepthStencilState* InteractiveEnv::depth_disabled_state() { 
  return depth_disabled_state_;
}
}  // namespace azer
