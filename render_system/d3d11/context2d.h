#pragma once

#include "base/basictypes.h"
#include "azer/render/context2d.h"

struct GrGLInterface;
class GrContext;
class GrTexture;

namespace azer {

class RenderSystem;

namespace d3d11 {

class D3DContext2D : public Context2D {
 public:
  D3DContext2D();
  virtual ~D3DContext2D();
  
  GrContext* GetGrContext() { return gr_context_;}
  GrTexture* CreateTexture(int width, int height);

  bool Init(RenderSystem* rs);

  virtual void flush() override;
  virtual void finish() override;
  virtual Canvas2D* CreateCanvas(int32 width, int32 height) override;
 private:
  GrContext* gr_context_;
  DISALLOW_COPY_AND_ASSIGN(D3DContext2D);
};
}  // namespace d3d11
}  // namespace azer
