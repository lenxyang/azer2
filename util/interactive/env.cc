#include "azer/util/interactive/env.h"

namespace azer {
namespace {
static LazyInstance<InteractiveEnv> env_instance = LAZY_INSTANCE_INITIALIZER;
}
static InteractiveEnv* InteractiveEnv::GetInstance() {
  return env_instance.Pointer();
}

InteractiveEnv::InteractiveEnv() {}
InteractiveEnv::~InteractiveEnv() {}
}  // namespace azer
