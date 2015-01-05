#include "azer/render_system/d3d11/context2d.h"

#include "third_party/skia/include/gpu/gl/GrGLInterface.h"
#include "third_party/skia/include/gpu/GrContext.h"

#define GL_GLEXT_PROTOTYPES
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

#include "base/logging.h"
#include "base/lazy_instance.h"
#include "base/command_line.h"
#include "ui/gl/gl_surface.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_share_group.h"
#include "ui/gl/gl_bindings_skia_in_process.h"
#include "ui/gl/gl_switches.h"

#include "azer/render_system/d3d11/canvas2d.h"

namespace azer {
namespace d3d11 {
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

namespace {
class GLSurfaceANGLE : public gfx::GLSurface {
 public:
  static bool Initialize() {
    CommandLine* cmd = CommandLine::ForCurrentProcess();
    bool fallback_to_osmesa = false;
    bool gpu_service_logging = cmd->HasSwitch(switches::kEnableGPUServiceLogging);
    bool disable_gl_drawing = cmd->HasSwitch(switches::kDisableGLDrawingForTests);
    return InitializeOneOffImplementation(
        gfx::kGLImplementationEGLGLES2,
        fallback_to_osmesa,
        gpu_service_logging,
        disable_gl_drawing);
  }
};
class CGLEnvironment {
 public:
  CGLEnvironment()
      : initialized_failed_(false)
      , initialized_(false) {
  }

  bool Init() {
    if (initialized_failed_) { return false;}
    if (initialized_) { return true;}

    // gfx::GLSurface::InitializeOneOff();
    CHECK(GLSurfaceANGLE::Initialize());
    surface_ = gfx::GLSurface::CreateOffscreenGLSurface(gfx::Size(1,1));
    if (!surface_.get()) {
      initialized_failed_= true;
      return false;
    }

    gl_share_group_ = new gfx::GLShareGroup;
    context_ = gfx::GLContext::CreateGLContext(
        gl_share_group_.get(), surface_.get(), gfx::PreferDiscreteGpu);
    context_->MakeCurrent(surface_.get());
    
    interface_ = gfx::CreateInProcessSkiaGLBinding();
    interface_->fFunctions.fGenerateMipmap = StubGLGenerateMipmap;
    interface_->fFunctions.fCompressedTexSubImage2D = StubGLCompressedTexSubImage2D;
    interface_->fFunctions.fMapBufferRange = StubGLMapBufferRangeEXT;
    interface_->fFunctions.fFlushMappedBufferRange = StubGLFlushMappedBufferRangeEXT;
    initialized_ = true;
    return true;
  }
  void finish() {
    DCHECK(NULL != interface_);
    interface_->fFunctions.fFinish();
  }
  GrGLInterface* GetGrGLInterface() { return interface_;}
 private:
  bool initialized_failed_;
  bool initialized_;
  scoped_refptr<gfx::GLShareGroup> gl_share_group_;
  scoped_refptr<gfx::GLContext> context_;
  scoped_refptr<gfx::GLSurface> surface_;
  GrGLInterface* interface_;
  DISALLOW_COPY_AND_ASSIGN(CGLEnvironment);
};

base::LazyInstance<CGLEnvironment>::Leaky gl_context_ = LAZY_INSTANCE_INITIALIZER;
}

// class Context2D
D3DContext2D::D3DContext2D()
    : gr_context_(NULL) {
  CHECK(gl_context_.Pointer()->Init());
}

D3DContext2D::~D3DContext2D() {
  if (gr_context_) {
    delete gr_context_;
  }
}

bool D3DContext2D::Init(RenderSystem* rs) {
  // code reference: skia/include/gpu/GrContextFactory.h
  
  GrBackendContext p3dctx =
    reinterpret_cast<GrBackendContext>(gl_context_.Pointer()->GetGrGLInterface());
  gr_context_ = GrContext::Create(kOpenGL_GrBackend, p3dctx);
  if (!gr_context_) {
    return false;
  }

  return true;
}

void D3DContext2D::flush() {
  DCHECK(gr_context_ != NULL);
  gr_context_->resetContext();
  gr_context_->flush();
}

void D3DContext2D::finish() {
  DCHECK(gr_context_ != NULL);
  flush();
  gl_context_.Pointer()->finish();
}

GrTexture* D3DContext2D::CreateTexture(int width, int height) {
  GrTextureDesc desc;
  desc.fConfig = kRGBA_8888_GrPixelConfig;
  desc.fFlags = kRenderTarget_GrTextureFlagBit;
  desc.fWidth = width;
  desc.fHeight = height;
  desc.fSampleCnt = 0;
  return gr_context_->createUncachedTexture(desc, NULL, 0);
}

Canvas2D* D3DContext2D::CreateCanvas(int32 width, int32 height) {
  std::unique_ptr<D3DCanvas2D> ptr(new D3DCanvas2D(width, height, this));
  if (ptr->Init()) {
    return ptr.release();
  } else {
    return NULL;
  }
}
}  // namespace d3d11
}  // namespace azer
