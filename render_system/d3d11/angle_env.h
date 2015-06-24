#pragma once

#include <memory>
#include "base/basictypes.h"
#include "azer/render/surface.h"
#include "base/lazy_instance.h"

class GrContext;

namespace azer {
namespace d3d11 {

class CGLEnvironment;
class AngleEnv {
 public:
  virtual ~AngleEnv();
  static AngleEnv* Pointer();

  GrContext* CreateGrContext();

  void finish();
  void flush();
  bool swap();

  bool GetProcAddress(const char* name, void** procaddr);

  // initialize
  bool InitForView(Surface* surface);
  bool InitForOffscreen();
 private:
  AngleEnv();

  friend class AngleD3DEnvironment;
  friend class InternalD3DEnvironment;
  friend struct ::base::DefaultLazyInstanceTraits<AngleEnv>;
  std::unique_ptr<CGLEnvironment> glenv_;
  DISALLOW_COPY_AND_ASSIGN(AngleEnv);
};

bool GetANGLEProcAddress(const char* name, void** procaddr);

}  // namespace d3d11
}  // namespace azer
