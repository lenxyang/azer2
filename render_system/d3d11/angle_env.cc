#include "azer/render_system/d3d11/angle_env.h"

#include "third_party/skia/include/gpu/gl/GrGLInterface.h"
#include "third_party/skia/include/gpu/GrContext.h"
#include "azer/render_system/d3d11/angle_interface.h"

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

namespace azer {
namespace d3d11 {

class GLSurfaceANGLE : public gfx::GLSurface {
 public:
  // to access protected memober of gfx::GLSurface
  // we just initialize of GLES2(angle)
  static bool Initialize();
};

class CGLEnvironment {
 public:
  CGLEnvironment();
  ~CGLEnvironment();

  bool InitForView(Surface* surface);
  bool InitForOffscreen();
  GrContext* CreateGrContext();
 private:
  bool initialized_failed_;
  bool initialized_;
  scoped_refptr<gfx::GLShareGroup> gl_share_group_;
  scoped_refptr<gfx::GLContext> context_;
  scoped_refptr<gfx::GLSurface> surface_;
  GrGLInterface* interface_;
  DISALLOW_COPY_AND_ASSIGN(CGLEnvironment);
};

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

bool GLSurfaceANGLE::Initialize() {
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

CGLEnvironment::CGLEnvironment()
    : initialized_failed_(false)
    , initialized_(false) {
}

CGLEnvironment::~CGLEnvironment() {
}

bool CGLEnvironment::InitForView(Surface* surface) {
  return false;
}

bool CGLEnvironment::InitForOffscreen() {
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

GrContext* CGLEnvironment::CreateGrContext() {
  // code reference: skia/include/gpu/GrContextFactory.h
  DCHECK(initialized_);
  GrBackendContext p3dctx = reinterpret_cast<GrBackendContext>(interface_);
  return GrContext::Create(kOpenGL_GrBackend, p3dctx);
}

AngleEnv::AngleEnv() {
  glenv_.reset(new CGLEnvironment());
}

AngleEnv::~AngleEnv() {
}

GrContext* AngleEnv::CreateGrContext() {
  return glenv_->CreateGrContext();
}

bool AngleEnv::InitForView(Surface* surface) {
  if (!glenv_->InitForView(surface)) {
    return false;
  }

  return InitANGLEInterface();
}

bool AngleEnv::InitForOffscreen() {
  if (!glenv_->InitForOffscreen()) {
    return false;
  }

  return InitANGLEInterface();
}

base::LazyInstance<AngleEnv>::Leaky angleenv_ = LAZY_INSTANCE_INITIALIZER;
AngleEnv* AngleEnv::Pointer() {
  return angleenv_.Pointer();
}

bool AngleEnv::GetProcAddress(const char* name, void** proc) {
  HMODULE module = GetModuleHandle(ANGLE_GLNAME);
  *proc = ::GetProcAddress(module, name);
  return *proc != NULL;
}

bool GetAngleProcAddress(const char* name, void** procaddr) {
  return AngleEnv::Pointer()->GetProcAddress(name, procaddr);
}
}  // namespace d3d11
}  // namespace azer
