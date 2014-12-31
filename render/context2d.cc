#include "azer/render/context2d.h"

#include "third_party/skia/src/gpu/gl/GrGLUtil.h"
#include "third_party/skia/include/gpu/gl/GrGLFunctions.h"
#include "third_party/skia/include/gpu/gl/GrGLInterface.h"
#include "third_party/skia/include/gpu/GrTexture.h"
#include "third_party/skia/include/gpu/GrContext.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkImageInfo.h"
#include "third_party/skia/include/core/SkImageEncoder.h"

#include "third_party/skia/src/gpu/SkGpuDevice.h"

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

void Context2D::flush() {
  DCHECK(gr_context_ != NULL);
  gr_context_->resetContext();
  gr_context_->flush();
}

void Context2D::finish() {
  DCHECK(gr_context_ != NULL);
  gr_context_->resetContext();
  interface_->fFunctions.fFinish();
}
}  // namespace azer
