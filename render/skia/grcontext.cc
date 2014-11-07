#include "azer/render/skia/grcontext.h"

#include "gl/GrGLInterface.h"
#include "azer/render/render_system.h"
#include "azer/render/egl.h"
#include "base/logging.h"

namespace azer {
namespace skia {
ASkGLContext::ASkGLContext(int width, int height)
    : width_(width), height_(height) {
  DCHECK_GT(width_, 0);
  DCHECK_GT(height_, 0);
  fGL.reset(createGLContext());
  if (fGL.get() == NULL) {
    destroyGLContext();
    DLOG(ERROR) << "Failed to create GLContext";
    return;
  }

  if (!fGL->validate()) {
    DLOG(ERROR) << "Could valid ANGLE interface.\n";
    destroyGLContext();
    return;
  }
}

ASkGLContext::~ASkGLContext() {
  destroyGLContext();
}

const GrGLInterface* ASkGLContext::createGLContext() {
  RenderSystem* rs = RenderSystem::Current();
  context_.width = width_;
  context_.height = height_;
  azer_egl_ = rs->GetEGLInterface();
  if (!azer_egl_) {
    return NULL;
  }
  if (!azer_egl_->Init(&context_)) {
    return NULL;
  }

  SkAutoTUnref<const GrGLInterface> grinterface(azer_egl_->AssimbleInterface());
  if (NULL == grinterface.get()) {
    LOG(ERROR) << "Failed to createANGLEInterface()";
    this->destroyGLContext();
    return NULL;
  }
  return GrGLInterfaceRemoveNVPR(grinterface.get());
}

void ASkGLContext::destroyGLContext() {
  fGL.reset(NULL);
  if (azer_egl_) {
    azer_egl_->Destroy(&context_);
  }
}

void ASkGLContext::makeCurrent() const {
  DCHECK(azer_egl_ != NULL);
  if (!azer_egl_->MakeCurrent(&context_)) {
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
