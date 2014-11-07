#pragma once

#include "base/basictypes.h"
#include "azer/render/glcontext.h"
#include "azer/render/texture.h"

struct GrGLInterface;

namespace azer {

/**
 * 实现类似于 EGL 的功能，用于帮助初始化 OpenGL 接口
 * 此处完成的功能是初始化 ANGLE
 */
class D3D11RenderSystem;
class AngleEGL : public AzerEGLInterface {
 public:
  AngleEGL(D3D11RenderSystem* rs)
      : render_system_(rs) {}
  ~AngleEGL() {}
  virtual bool Init(AzerEGLContext* context) override;
  virtual bool MakeCurrent(const AzerEGLContext* context) override;
  virtual void Destroy(AzerEGLContext* context) override;
  virtual Texture* GetShareTextureFromTex(uint32 texid) override;
  virtual const GrGLInterface* AssimbleInterface() override;
 private:
  void* module_;
  GrGLInterface* interface_;
  D3D11RenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(AngleEGL);
};
}  // namespace azer
