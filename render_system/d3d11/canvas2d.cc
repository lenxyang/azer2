#include "azer/render_system/d3d11/canvas2d.h"

#include <windows.h>

#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

#include "SkCanvas.h"
#include "SkGpuDevice.h"
#include "GrTexture.h"
#include "GrContext.h"

#include "azer/render_system/d3d11/device2d.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render/render_system.h"


extern "C" {
typedef void (*FUNCGLGETTEXSHARED3DTEX)(GLenum target, GLuint fbhandle, void** val);
}

namespace azer {
namespace d3d11 {

namespace {
FUNCGLGETTEXSHARED3DTEX fnglGetTexShareD3DTexProc = NULL;

bool GetProc() {
  if (fnglGetTexShareD3DTexProc) {
    return true;
  }

  HMODULE module = GetModuleHandle(ANGLE_GLNAME);
  fnglGetTexShareD3DTexProc = (FUNCGLGETTEXSHARED3DTEX)
      ::GetProcAddress(module, "glGetTexShareD3DTex");
  if (fnglGetTexShareD3DTexProc) {
    return true;
  } else {
    return false;
  }
}
}  // namespace

bool Canvas2D::Init() {
  std::unique_ptr<D3DDevice> device(new D3DDevice2D());
  if (!device->Init(context_, this)) {
    return false;
  }

  device_.reset(device.release());
  GrTexture* tex = device_->GetGrTex();
  return InitTexture(tex->getTextureHandle());
}

bool D3DCanvas2D::InitTexture(int32 texid) {
  if (!GetProc()) {
    return TexturePtr();
  }

  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  HANDLE handle = 0;
  (*fnglGetTexShareD3DTexProc)(GL_DRAW_FRAMEBUFFER_ANGLE, texid, &handle);
  texture_ =  TexturePtr(D3DTexture2DExtern::Create(handle, rs));
  if (texture_.get()) {
    return true;
  } else {
    return false;
  }
}

}  // namespace d3d11
}  // namespace azer
