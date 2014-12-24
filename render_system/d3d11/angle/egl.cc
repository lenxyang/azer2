#include "azer/render_system/d3d11/angle/egl.h"

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

#include "gl/GrGLInterface.h"          // skia

#include "azer/ui/window/window_host.h"
#include "azer/render/surface.h"
#include "azer/render_system/d3d11/angle/module.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/render_system.h"

extern "C" {
typedef void (*FUNCGLGETTEXSHARED3DTEX)(GLenum target, GLuint fbhandle, void** val);
}

namespace azer {
FUNCGLGETTEXSHARED3DTEX glGetTexShareD3DTexProc;

bool AngleEGL::Init() {
  EGLint numConfigs;
  EGLint majorVersion;
  EGLint minorVersion;
  EGLDisplay display;
  EGLContext context;
  EGLSurface surface;
  EGLConfig config;
  EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

  SurfacePtr& azer_surface = render_system_->GetSurface();
  EGLNativeWindowType hWnd = (EGLNativeWindowType)azer_surface->GetWindow();
  display = eglGetDisplay(GetDC(hWnd));
  if (display == EGL_NO_DISPLAY) {
    return false;
  }

  // Initialize EGL
  if (!eglInitialize(display, &majorVersion, &minorVersion)) {
    return false;
  }

  // Get configs
  if (!eglGetConfigs(display, NULL, 0, &numConfigs)) {
    return false;
  }

  // Choose config
  const int kMaxEGLAttrNum = 256;
  EGLint device_config_list[] = {
    EGL_RED_SIZE,       8,
    EGL_GREEN_SIZE,     8,
    EGL_BLUE_SIZE,      8,
    EGL_ALPHA_SIZE,     8,
    EGL_DEPTH_SIZE,     24,
    EGL_STENCIL_SIZE,   8,
    EGL_SAMPLE_BUFFERS, 0,
    EGL_NONE, EGL_NONE
  };
  if (!eglChooseConfig(display, device_config_list, &config, 1, &numConfigs)) {
    return false;
  }

  EGLint surfaceAttribList[] =  {
    EGL_WIDTH,   1,
    EGL_HEIGHT,  1,
    EGL_NONE, EGL_NONE
  };
  surface = eglCreatePbufferSurface(display, config, surfaceAttribList);
  if (surface == EGL_NO_SURFACE) {
    LOG(ERROR) << "Failed to create Surface";
    return false;
  }
  
  // Create a GL context
  context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
  if (context == EGL_NO_CONTEXT) {
    return EGL_FALSE;
  }

  // Make the context current
  if (!eglMakeCurrent(display, surface, surface, context)) {
    return EGL_FALSE;
  }

  context_.display = display;
  context_.surface = surface;
  context_.context = context;

  module_ = ANGLEModule::GetInstance();
  if (NULL == module_ || !module_->GetModule()) {
    return false;
  }

  glGetTexShareD3DTexProc =
      (FUNCGLGETTEXSHARED3DTEX)module_->GetProcAddress("glGetTexShareD3DTex");
  if (!glGetTexShareD3DTexProc) {
    return false;
  }

  return true;
}

void* AngleEGL::GetProcAddress(const char* name) {
  DCHECK(module_ != NULL && module_->GetModule() != NULL);
  return module_->GetProcAddress(name);
}

bool AngleEGL::MakeCurrent() {
  EGLContext context = (EGLContext)context_.context;
  EGLDisplay display = (EGLDisplay)context_.display;
  EGLSurface surface = (EGLSurface)context_.surface;
  return EGL_FALSE == eglMakeCurrent(display, surface, surface, context);
}

void AngleEGL::Destroy() {
  if (context_.display) {
    eglMakeCurrent((EGLDisplay)context_.display, 0, 0, 0);
    if (context_.context) {
      eglDestroyContext((EGLDisplay)context_.display, (EGLContext)context_.context);
    }

    if (context_.surface) {
      eglDestroySurface((EGLDisplay)context_.display, (EGLSurface)context_.surface);
    }
  }
}

Texture* AngleEGL::GetShareTextureFromTex(uint32 texid) {
  DCHECK(glGetTexShareD3DTexProc != NULL);
  D3D11RenderSystem* rs = render_system_;
  HANDLE handle = 0;
  (*glGetTexShareD3DTexProc)(GL_DRAW_FRAMEBUFFER_ANGLE, texid, &handle);
  return D3D11Texture2DExtern::Create(handle, rs);
}
}  // namespace
