#include "azer/render_system/d3d11/canvas2d.h"

#include "ui/gl/gl_implementation.h"

#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"


extern "C" {
typedef void (*FUNCGLGETTEXSHARED3DTEX)(GLenum target, GLuint fbhandle, void** val);
}

namespace azer {
namespace d3d11 {

namespace {
FUNCGLGETTEXSHARED3DTEX fnglGetTexShareD3DTexProc = NULL;
}  // namespace

TexturePtr D3DCanvas2D::InitTexture() {
  if (!fnglGetTexShareD3DTexProc) {
    fnglGetTexShareD3DTexProc = gfx::GetProcAddress("glGetTexShareD3DTex");
  }

  if (!fnglGetTexShareD3DTexProc) {
    return TexturePtr();
  }

  (*glGetTexShareD3DTexProc)(GL_DRAW_FRAMEBUFFER_ANGLE, texid, &handle);

  return TexturePtr(D3DTexture2DExtern::Create(handle, rs));
}

}  // namespace d3d11
}  // namespace azer
