#include "azer/render_system/d3d11/canvas2d.h"

#include <windows.h>

#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

#include "azer/render_system/d3d11/context2d.h"
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

D3DCanvas2D::D3DCanvas2D(int width, int height, Context2D* context)
    : Canvas2D(width, height, context) {
}

D3DCanvas2D::~D3DCanvas2D() {
  if (skcanvas_) {
    delete skcanvas_;
  }
}

bool D3DCanvas2D::Init() {
  if (!InitCanvas()) {
    return false;
  }

  return InitTexture();
}

bool D3DCanvas2D::InitTexture() {
  if (!GetProc()) {
    return false;
  }

  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();

  DCHECK(NULL != gr_device_.get());
  DCHECK(NULL != grtex_.get());
  D3DContext2D* context = (D3DContext2D*)GetContext2D();
  int32 texid = context->GetRenderTargetColorTexID(this);
  // int32 texid = grtex_->getTextureHandle();
  
  HANDLE handle = 0;
  (*fnglGetTexShareD3DTexProc)(GL_DRAW_FRAMEBUFFER_ANGLE, texid, &handle);
  texture_ = TexturePtr(D3DTexture2DExtern::Create(handle, rs));
  return true;
}

bool D3DCanvas2D::InitCanvas() {
  D3DContext2D* ctx = (D3DContext2D*)context_;
  GrContext* context =  ctx->GetGrContext();
  DCHECK(NULL != context);
  grtex_.reset(ctx->CreateTexture(width_, height_));
  SkSurfaceProps props(SkSurfaceProps::kDisallowAntiAlias_Flag,
                       kRGB_H_SkPixelGeometry);
  gr_device_.reset(SkGpuDevice::Create(grtex_.get(), props, 0));
  if (gr_device_.get() == NULL) {
    LOG(ERROR) << "Failed to create SkGpuDevice";
    return false;
  }

  DCHECK(NULL == skcanvas_);
  skcanvas_ = new SkCanvas(gr_device_.get());
  if (skcanvas_ == NULL) {
    LOG(ERROR) << "Failed to create SkCanvas";
    return false;
  }

  skcanvas_->clear((SkColor)0x01010101);
  skcanvas_->clear((SkColor)0);

  return true;
}

uint32 D3DCanvas2D::GetTexID() {
  DCHECK(NULL != grtex_.get());
  return grtex_->getTextureHandle();
}
}  // namespace d3d11
}  // namespace azer
