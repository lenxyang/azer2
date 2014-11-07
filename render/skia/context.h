#pragma once

#include <memory>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "azer/render/egl.h"
#include "azer/render/render_system.h"
#include "azer/render/skia/canvas.h"

struct GrGLInterface;
class GrContext;
class SkGpuDevice;
class SkCanvas;

namespace azer {

class RenderSystem;

namespace skia {

class ASkGLContext;
class Context;
class AzerSkDevice;

class AZER_EXPORT Context {
 public:
  Context();
  ~Context();

  bool Init(RenderSystem* rs);
  void flush();
  void wait();

  CanvasPtr CreateCanvas(int width, int height);

  EGL* GetEGL();

  GrContext* GetGrContext() { return gr_context_;}
  const GrGLInterface* GetGrGLInterface() { return interface_;}
 private:
  std::unique_ptr<EGL> egl_;
  GrContext* gr_context_;
  const GrGLInterface* interface_;
  ASkGLContext* helper_;
  friend class AzerSkDevice;
  DISALLOW_COPY_AND_ASSIGN(Context);
};

inline EGL* Context::GetEGL() {
  DCHECK(egl_.get() != NULL);
  return egl_.get();
}

}  // namespace skia
}  // namespace azer
