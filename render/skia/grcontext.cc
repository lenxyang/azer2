#include "azer/render/skia/grcontext.h"

#include "gl/GrGLInterface.h"
#include "azer/render/skia/GrGLUtil.h"
#include "azer/render/skia/GrGLAssembleInterface.h"
#include "azer/render/render_system.h"
#include "azer/render/egl.h"
#include "base/logging.h"

namespace azer {
namespace skia {

namespace {
const GrGLInterface* RemoveGrGLInterfaceNVPR(const GrGLInterface* interface) {
    GrGLInterface* newInterface = GrGLInterface::NewClone(interface);

    newInterface->fExtensions.remove("GL_NV_path_rendering");
    newInterface->fFunctions.fPathCommands = NULL;
    newInterface->fFunctions.fPathCoords = NULL;
    newInterface->fFunctions.fPathParameteri = NULL;
    newInterface->fFunctions.fPathParameterf = NULL;
    newInterface->fFunctions.fGenPaths = NULL;
    newInterface->fFunctions.fDeletePaths = NULL;
    newInterface->fFunctions.fIsPath = NULL;
    newInterface->fFunctions.fPathStencilFunc = NULL;
    newInterface->fFunctions.fStencilFillPath = NULL;
    newInterface->fFunctions.fStencilStrokePath = NULL;
    newInterface->fFunctions.fStencilFillPathInstanced = NULL;
    newInterface->fFunctions.fStencilStrokePathInstanced = NULL;
    newInterface->fFunctions.fPathTexGen = NULL;
    newInterface->fFunctions.fCoverFillPath = NULL;
    newInterface->fFunctions.fCoverStrokePath = NULL;
    newInterface->fFunctions.fCoverFillPathInstanced = NULL;
    newInterface->fFunctions.fCoverStrokePathInstanced = NULL;
    newInterface->fFunctions.fStencilThenCoverFillPath = NULL;
    newInterface->fFunctions.fStencilThenCoverStrokePath = NULL;
    newInterface->fFunctions.fStencilThenCoverFillPathInstanced = NULL;
    newInterface->fFunctions.fStencilThenCoverStrokePathInstanced = NULL;
    newInterface->fFunctions.fProgramPathFragmentInputGen = NULL;
    newInterface->fFunctions.fPathMemoryGlyphIndexArray = NULL;
    return newInterface;
}

GrGLFuncPtr angle_get_gl_proc(void* ctx, const char name[]) {
  EGL* egl = (EGL*)ctx;
  return (GrGLFuncPtr)egl->GetProcAddress(name);
}
}

ASkGLContext::ASkGLContext(EGL* egl) :
    egl_(egl) {
}

ASkGLContext::~ASkGLContext() {
  destroyGLContext();
}


bool ASkGLContext::Init() {
  DCHECK(NULL != egl_);
  SkAutoTUnref<const GrGLInterface> intf(
      GrGLAssembleGLESInterface(egl_, angle_get_gl_proc));
  if (NULL == intf.get()) {
    LOG(ERROR) << "Failed to createANGLEInterface()";
    this->destroyGLContext();
    return false;
  }
  fGL.reset(RemoveGrGLInterfaceNVPR(intf.get()));
  // fGL.reset(GrGLInterfaceRemoveNVPR(intf.get()));
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
