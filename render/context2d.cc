#include "azer/render/context2d.h"

#include "third_party/skia/include/gpu/gl/GrGLInterface.h"
#include "third_party/skia/include/gpu/GrContext.h"

#include "base/logging.h"

#define GL_GLEXT_PROTOTYPES
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

namespace azer {

void StubGLGenerateMipmap(GLenum target) {
  glGenerateMipmap(target);
}

void StubGLCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, 
                                   GLint yoffset, GLsizei width, GLsizei height,
                                   GLenum format, GLsizei imageSize, 
                                   const GLvoid* data) {
  glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height,
                            format, imageSize, data);
}

GLvoid* StubGLMapBufferRangeEXT(GLenum target, GLintptr offset, 
                             GLsizeiptr length, GLbitfield access) {
  return glMapBufferRangeEXT(target, offset, length, access);
}

void StubGLFlushMappedBufferRangeEXT(GLenum target, GLintptr offset, 
                                     GLsizeiptr length) {
  glFlushMappedBufferRangeEXT(target, offset, length);
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
  interface_->fFunctions.fCompressedTexSubImage2D = StubGLCompressedTexSubImage2D;
  interface_->fFunctions.fMapBufferRange = StubGLMapBufferRangeEXT;
  interface_->fFunctions.fFlushMappedBufferRange = StubGLFlushMappedBufferRangeEXT;
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
