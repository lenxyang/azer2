#include "azer/render/context2d.h"

#include "third_party/skia/include/gpu/gl/GrGLInterface.h"
#include "third_party/skia/include/gpu/GrContext.h"

#include "base/logging.h"

#include "GLES2/gl2.h"

namespace azer {

void StubGLGenerateMipmap(GLenum target) {
  glGenerateMipmap(target);
}

// class Context2D
Context2D::Context2D(GrGLInterface* interface)
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

  interface_->fFunctions.fGenerateMipmap = StubGLGenerateMipmap;
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

GrTexture* Context2D::CreateTexture(int width, int height) {
  GrTextureDesc desc;
  desc.fConfig = kRGBA_8888_GrPixelConfig;
  desc.fFlags = kRenderTarget_GrTextureFlagBit;
  desc.fWidth = width;
  desc.fHeight = height;
  desc.fSampleCnt = 0;
  return gr_context_->createUncachedTexture(desc, NULL, 0);
}
}  // namespace azer
