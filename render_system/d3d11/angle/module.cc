
#include "azer/render_system/d3d11/angle/module.h"
#include "azer/base/string.h"
#include "base/lazy_instance.h"

namespace azer {
namespace {
base::LazyInstance<ANGLEModule>::Leaky gmodule;
}

ANGLEModule* ANGLEModule::GetInstance() {
  return gmodule.Pointer();
}

ANGLEModule::ANGLEModule() {
  module_ = LoadLibrary(ANGLE_GLNAME);
  CHECK(module_ != NULL) << "Failed to init module.";
}

ANGLEModule::~ANGLEModule() {
}
}  // namespace azer
