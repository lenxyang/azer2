#include "azer/render/skia/context.h"

#include "gl/GrGLFunctions.h"
#include "gl/GrGLDefines.h"
#include "gl/GrGLInterface.h"
#include "SkCanvas.h"
#include "SkGpuDevice.h"
#include "GrTexture.h"
#include "GrContext.h"
#include "SkImageInfo.h"
#include "SkImageEncoder.h"
#include "azer/render/skia/grcontext.h"
#include "azer/render/skia/GrGLUtil.h"

namespace azer {
namespace skia {

// class Context
Context::Context()
    : gr_context_(NULL)
    , interface_(NULL)
    , helper_(NULL) {
}

Context::~Context() {
  if (gr_context_) {
    delete gr_context_;
  }

  if (helper_) {
    delete helper_;
  }
}

bool Context::Init(RenderSystem* rs) {
  // code reference: skia/include/gpu/GrContextFactory.h
  egl_.reset(rs->CreateEGL());
  if (!egl_.get()) {
    return false;
  }
  if (!egl_->Init()) {
    return false;
  }

  helper_ = new ASkGLContext(egl_.get());
  if (helper_->Init()) {
    return false;
  }

  SkGLContext* glctx = helper_;
  interface_ = helper_->GetGrGlInterface();
  if (!interface_) {
    return false;
  }

  glctx->makeCurrent();
  GrBackendContext p3dctx = reinterpret_cast<GrBackendContext>(interface_);
  gr_context_ = GrContext::Create(kOpenGL_GrBackend, p3dctx);
  if (!gr_context_) {
    return false;
  }

  return true;
}

CanvasPtr Context::CreateCanvas(int width, int height) {
  std::unique_ptr<Canvas> ptr(new Canvas(width, height, this));
  if (ptr->Init()) {
    return CanvasPtr(ptr.release());
  } else {
    return CanvasPtr();
  }
}

void Context::flush() {
  DCHECK(gr_context_ != NULL);
  gr_context_->resetContext();
  gr_context_->flush();
}

void Context::wait() {
  DCHECK(gr_context_ != NULL);
  gr_context_->resetContext();
  GR_GL_CALL(interface_, Finish());
}
}  // namespace skia
}  // namespace azer
