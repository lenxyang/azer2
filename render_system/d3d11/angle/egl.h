#pragma once

#include "base/basictypes.h"
#include "azer/render/egl.h"
#include "azer/render/texture.h"

struct GrGLInterface;

namespace azer {
namespace d3d11 {

class ANGLEModule;

/**
 * 实现类似于 EGL 的功能，用于帮助初始化 OpenGL 接口
 * 此处完成的功能是初始化 ANGLE
 */
class D3DRenderSystem;
class AngleEGL : public EGL {
 public:
  AngleEGL(D3DRenderSystem* rs)
      : render_system_(rs) {}
  ~AngleEGL() {}
  virtual bool Init() override;
  virtual bool MakeCurrent() override;
  virtual void Destroy() override;
  virtual Texture* GetShareTextureFromTex(uint32 texid) override;
  virtual void* GetProcAddress(const char* name) override;  
 private:
  ANGLEModule* module_;
  GrGLInterface* interface_;
  D3DRenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(AngleEGL);
};
}  // namespace d3d11
}  // namespace azer
