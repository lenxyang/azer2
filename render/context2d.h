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

class AZER_EXPORT Context2D {
 public:
  virtual ~Context2D();
  
  void flush();
  void wait();

  Canvas2DPtr CreateCanvas(int width, int height);

  GrContext* GetGrContext() { return gr_context_;}
  const GrGLInterface* GetGrGLInterface() { return interface_;}
 private:
  Context2D(const GrGLInterface* interface);
  bool Init(RenderSystem* rs);

  GrContext* gr_context_;
  const GrGLInterface* interface_;
  friend class Device2D;
  friend class RenderSystem;
  DISALLOW_COPY_AND_ASSIGN(Context2D);
};

typedef std::shared_ptr<Context2D> Context2DPtr;

}  // namespace azer
