#include "azer/render_system/d3d11/context2d.h"

#include "azer/render_system/d3d11/canvas2d.h"
#include "azer/render_system/d3d11/angle_env.h"

#include "GLES2/gl2.h"

namespace azer {
namespace d3d11 {

// class Context2D
D3DContext2D::D3DContext2D()
    : gr_context_(NULL) {
}

D3DContext2D::~D3DContext2D() {
  if (gr_context_) {
    delete gr_context_;
  }
}

bool D3DContext2D::Init(RenderSystem* rs) {
  gr_context_ = AngleEnv::Pointer()->CreateGrContext();
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
  glFinish();
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

int32 D3DContext2D::GetRenderTargetColorTexID(D3DCanvas2D* canvas) {
  GrRenderTarget* target = canvas->GetSkGpuDevice()->accessRenderTarget();
  uint32 fboid = target->getRenderTargetHandle();

  GLint texid = 0;
  glBindFramebuffer(GL_FRAMEBUFFER, fboid);
  glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                        GL_COLOR_ATTACHMENT0,
                                        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                        &texid);
  gr_context_->resetContext();
  GLenum glerr = glGetError();
  if (glerr == GL_NO_ERROR) {
    return static_cast<int32>(texid);
  } else {
    return -1;
  }
}

}  // namespace d3d11
}  // namespace azer
