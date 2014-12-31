#include "azer/render/context2d.h"

#include "third_party/skia/src/gpu/gl/GrGLUtil.h"
#include "gl/GrGLFunctions.h"
#include "gl/GrGLDefines.h"
#include "gl/GrGLInterface.h"
#include "SkCanvas.h"
#include "SkGpuDevice.h"
#include "GrTexture.h"
#include "GrContext.h"
#include "SkImageInfo.h"
#include "SkImageEncoder.h"

namespace azer {

// class Context2D
Context2D::Context2D(const GrGLInterface* interface)
    : gr_context_(NULL)
    , interface_(interface) {
}

Context2D::~Context2D() {
  if (gr_context_) {
    delete gr_context_;
  }
}

bool Context2D::Init(RenderSystem* rs) {
  // code reference: skia/include/gpu/GrContextFactory.h
  DCHECK(NULL != interface_);

  GrBackendContext p3dctx = reinterpret_cast<GrBackendContext>(interface_);
  gr_context_ = GrContext::Create(kOpenGL_GrBackend, p3dctx);
  if (!gr_context_) {
    return false;
  }

  return true;
}

Canvas2DPtr Context2D::CreateCanvas(int width, int height) {
  std::unique_ptr<Canvas2D> ptr(new Canvas2D(width, height, this));
  if (ptr->Init()) {
    return Canvas2DPtr(ptr.release());
  } else {
    return Canvas2DPtr();
  }
}

void Context2D::flush() {
  DCHECK(gr_context_ != NULL);
  gr_context_->resetContext();
  gr_context_->flush();
}

void Context2D::wait() {
  DCHECK(gr_context_ != NULL);
  gr_context_->resetContext();
  interface_->fFunctions.fFinish();
}
}  // namespace azer
