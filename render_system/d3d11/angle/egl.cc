#include "azer/render_system/d3d11/angle/egl.h"


#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
#include "libGLESv2/main.h"
#include "gl/GrGLInterface.h"          // skia
#include "gl/GrGLAssembleInterface.h"  // skia
#include "azer/render_system/d3d11/angle/angle.h"
#include "azer/render_system/d3d11/angle/module.h"
#include "azer/render_system/d3d11/texture.h"

namespace azer {

bool AngleEGL::Init(AzerEGLContext* context) {
  if (! angle::Init((RenderSystem*)render_system_, context)) {
    return false;
  }

  module_ = (void*)ANGLEModule::GetInstance()->GetModule();
  if (NULL == module_) {
    return false;
  }
  return true;
}

bool AngleEGL::MakeCurrent(const AzerEGLContext* ctx) {
  EGLContext context = (EGLContext)ctx->context;
  EGLDisplay display = (EGLDisplay)ctx->display;
  EGLSurface surface = (EGLSurface)ctx->surface;
  return EGL_FALSE == eglMakeCurrent(display, surface, surface, context);
}

void AngleEGL::Destroy(AzerEGLContext* context) {
  angle::Destroy(context);
}

Texture* AngleEGL::GetShareTextureFromTex(uint32 texid) {
  D3D11RenderSystem* rs = render_system_;
  HANDLE handle = 0;
  glGetTexShareD3DTex(GL_DRAW_FRAMEBUFFER_ANGLE, texid, &handle);
  return D3D11Texture2DExtern::Create(handle, rs);
}

static GrGLFuncPtr angle_get_gl_proc(void* ctx, const char name[]) {
  GrGLFuncPtr proc = (GrGLFuncPtr) GetProcAddress((HMODULE)ctx, name);
  if (proc) {
    return proc;
  }
  return eglGetProcAddress(name);
}


const GrGLInterface* AngleEGL::AssimbleInterface() {
  HMODULE module = (HMODULE)module_;
  return GrGLAssembleGLESInterface(module_, angle_get_gl_proc);
}
}  // namespace
