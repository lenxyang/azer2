#include "azer/render/skia/grcontext.h"

#include "gl/GrGLInterface.h"
#include "azer/render/render_system.h"
#include "azer/render/egl.h"
#include "base/logging.h"

namespace azer {
namespace skia {
ASkGLContext::ASkGLContext(EGL* egl) :
    egl_(egl) {
}

ASkGLContext::~ASkGLContext() {
  destroyGLContext();
}

bool ASkGLContext::Init() {
  SkAutoTUnref<const GrGLInterface> intf(egl_->AssimbleInterface());
  if (NULL == intf.get()) {
    LOG(ERROR) << "Failed to createANGLEInterface()";
    this->destroyGLContext();
    return false;
  }
  fGL.reset(GrGLInterfaceRemoveNVPR(intf.get()));
  if (fGL.get() == NULL) {
    destroyGLContext();
    DLOG(ERROR) << "Failed to create GLContext";
    return false;
  }

  if (!fGL->validate()) {
    DLOG(ERROR) << "Could valid ANGLE interface.\n";
    destroyGLContext();
    return false;
  }

  return true;
}

void ASkGLContext::destroyGLContext() {
  fGL.reset(NULL);
  if (egl_) {
    egl_->Destroy();
  }
}

void ASkGLContext::makeCurrent() const {
  DCHECK(egl_ != NULL);
  if (!egl_->MakeCurrent()) {
    DLOG(ERROR) << "cannot set context!";
  }

  DLOG(INFO) << "egl::makeCurrent on ANGLE interface!";
}

void ASkGLContext::swapBuffers() const {
/*
  EGLContext context = (EGLContext)context_.context;
  EGLDisplay display = (EGLDisplay)context_.display;
  EGLSurface surface = (EGLSurface)context_.surface;
  if (!eglSwapBuffers(display, surface)) {
    LOG(INFO) << "cannot not complete eglSwwapBuffers!";
  }
*/
}
}  // namespace skia
}  // namespace azer
