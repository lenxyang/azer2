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
namespace skia {

class ASkGLContext;
class Context;
class AzerSkDevice;

class AZER_EXPORT Context {
 public:
  Context();
  ~Context();

  bool Init();
  void flush();
  void wait();

  CanvasPtr CreateCanvas(int width, int height);

  AzerEGLContext* GetAzerEGLContext();
  AzerEGLInterface* GetAzerEGLInterface();

  GrContext* GetGrContext() { return gr_context_;}
  const GrGLInterface* GetGrGLInterface() { return interface_;}
 private:
  GrContext* gr_context_;
  const GrGLInterface* interface_;
  ASkGLContext* helper_;
  friend class AzerSkDevice;
  DISALLOW_COPY_AND_ASSIGN(Context);
};

}  // namespace skia
}  // namespace azer
