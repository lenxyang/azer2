
#include "azer/render_system/d3d11/angle/module.h"

#include "EGL/egl.h"
#include "EGL/eglext.h"

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "azer/base/string.h"
#include "base/lazy_instance.h"

namespace azer {
namespace d3d11 {
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

void* ANGLEModule::GetProcAddress(const std::string& name) {
  DCHECK(module_ != NULL);
  void *func = ::GetProcAddress((HMODULE)module_, name.c_str());
  if (func) {
    return func;
  }

  return eglGetProcAddress(name.c_str());
}

}  // namespace d3d11
}  // namespace azer
