#pragma once


#include "azer/render/context2d.h"

class GrContext;
class GrTexture;
class AngleEnv;

namespace azer {

class RenderSystem;

namespace d3d11 {

class AngleEnv;
class D3DCanvas2D;

class D3DContext2D : public Context2D {
 public:
  D3DContext2D();
  virtual ~D3DContext2D();
  
  GrContext* GetGrContext() { return gr_context_;}
  GrTexture* CreateTexture(int width, int height);

  bool Init(RenderSystem* rs);

  virtual void flush() override;
  virtual void finish() override;
  virtual Canvas2D* CreateCanvas(int32_t width, int32_t height) override;

  int32_t GetRenderTargetColorTexID(D3DCanvas2D* canvas);
 private:
  GrContext* gr_context_;
  DISALLOW_COPY_AND_ASSIGN(D3DContext2D);
};
}  // namespace d3d11
}  // namespace azer
