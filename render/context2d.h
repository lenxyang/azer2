#pragma once

#include <memory>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "azer/render/render_system.h"
#include "azer/render/canvas2d.h"

struct GrGLInterface;
class GrContext;
class SkGpuDevice;
class SkCanvas;

namespace azer {

class RenderSystem;
class Context2D;
class Device2D;
class EGL;

namespace skia {
class ASkGLContext;
}  // namespace skia

class AZER_EXPORT Context2D {
 public:
  Context2D();
  ~Context2D();

  bool Init(RenderSystem* rs);
  void flush();
  void wait();

  Canvas2DPtr CreateCanvas(int width, int height);

  EGL* GetEGL();

  GrContext* GetGrContext() { return gr_context_;}
  const GrGLInterface* GetGrGLInterface() { return interface_;}
 private:
  std::unique_ptr<EGL> egl_;
  GrContext* gr_context_;
  const GrGLInterface* interface_;
  skia::ASkGLContext* helper_;
  friend class Device2D;
  DISALLOW_COPY_AND_ASSIGN(Context2D);
};

inline EGL* Context2D::GetEGL() {
  DCHECK(egl_.get() != NULL);
  return egl_.get();
}

}  // namespace azer
