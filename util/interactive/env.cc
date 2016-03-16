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
}

InteractiveEnv::~InteractiveEnv() {}
Effect* InteractiveEnv::GetEffect(const std::string& name) {
  return effectlib_->GetEffect(name);
}
EffectAdapterContext* InteractiveEnv::effect_context() {
  return effectlib_->adapter_context();
}
}  // namespace azer
