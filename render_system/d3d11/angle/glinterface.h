#pragma once

#include "base/basictypes.h"
#include "azer/render/glcontext.h"
#include "azer/render/texture.h"

namespace azer {
class D3D11RenderSystem;
class ANGLEGLInterface : public AzerEGLInterface {
 public:
  ANGLEGLInterface(D3D11RenderSystem* rs)
      : render_system_(rs) {}
  ~ANGLEGLInterface() {}
  virtual bool Init(AzerEGLContext* context) override;
  virtual bool MakeCurrent(const AzerEGLContext* context) override;
  virtual void Destroy(AzerEGLContext* context) override;
  virtual Texture* GetShareTextureFromTex(uint32 texid) override;
 private:
  D3D11RenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(ANGLEGLInterface);
};
}  // namespace azer
