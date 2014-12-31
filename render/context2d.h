#pragma once

#include <memory>

#include "base/basictypes.h"
#include "azer/base/export.h"

class GrContext;
struct GrGLInterface;

namespace azer {

class RenderSystem;
class Device2D;

class AZER_EXPORT Context2D {
 public:
  Context2D(GrGLInterface* interface);
  virtual ~Context2D();
  
  void flush();
  void finish();

  GrContext* GetGrContext() { return gr_context_;}
  GrGLInterface* GetGrGLInterface() { return interface_;}
 protected:
  virtual bool Init(RenderSystem* rs);

  GrContext* gr_context_;
  GrGLInterface* interface_;
  friend class Device2D;
  friend class RenderSystem;
  DISALLOW_COPY_AND_ASSIGN(Context2D);
};

typedef std::shared_ptr<Context2D> Context2DPtr;

}  // namespace azer
