#pragma once

#include <memory>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "azer/render/render_system.h"
#include "azer/render/canvas2d.h"

#include "third_party/skia/include/gpu/gl/GrGLInterface.h"

struct GrGLInterface;
class GrContext;


namespace azer {

class RenderSystem;
class Context2D;
class Device2D;

class AZER_EXPORT Context2D {
 public:
  Context2D(const GrGLInterface* interface);
  virtual ~Context2D();
  
  void flush();
  void finish();

  GrContext* GetGrContext() { return gr_context_;}
  const GrGLInterface* GetGrGLInterface() { return interface_;}
 protected:
  bool Init(RenderSystem* rs);

  GrContext* gr_context_;
  const GrGLInterface* interface_;
  friend class Device2D;
  friend class RenderSystem;
  DISALLOW_COPY_AND_ASSIGN(Context2D);
};

typedef std::shared_ptr<Context2D> Context2DPtr;

}  // namespace azer
