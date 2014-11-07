#pragma once

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include "base/basictypes.h"
#include "base/lazy_instance.h"
#include "azer/render_system/d3d11/export.h"

namespace azer {
class AZER_D3D11RS_EXPORT ANGLEModule {
 public:
  static ANGLEModule* GetInstance();

  HMODULE GetModule() { return module_;}
 private:
  ANGLEModule();
  ~ANGLEModule();

  HMODULE module_;
  friend struct ::base::DefaultLazyInstanceTraits<ANGLEModule>;
  DISALLOW_COPY_AND_ASSIGN(ANGLEModule);
};
}  // namespace azer
