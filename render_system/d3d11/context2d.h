#pragma once

#include "base/basictypes.h"
#include "azer/render/context2d.h"

class GrContext;
class GrTexture;
struct GrGLInterface;

namespace azer {

class RenderSystem;

namespace d3d11 {

class D3DContext2D {
 public:
  D3DContext2D();
  ~D3DContext2D();
  GrContext* GetGrContext() { return gr_context_;}
  GrGLInterface* GetGrGLInterface() { return interface_;}
  GrTexture* CreateTexture(int width, int height);
 private:
  GrContext* gr_context_;
  GrGLInterface* interface_;
  DISALLOW_COPY_AND_ASSIGN(D3DContext2D);
};
}  // namespace d3d11
}  // namespace azer
