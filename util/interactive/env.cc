#include "azer/util/interactive/env.h"

#include "azer/effect/effectlib.h"


namespace azer {
namespace {
::base::LazyInstance<InteractiveEnv> env_instance = LAZY_INSTANCE_INITIALIZER;
}
InteractiveEnv* InteractiveEnv::GetInstance() {
  return env_instance.Pointer();
}

InteractiveEnv::InteractiveEnv() {}

InteractiveEnv::~InteractiveEnv() {}
}  // namespace azer
