#pragma once

#include <memory>

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace azer {

class RenderSystem;
class Device2D;

class AZER_EXPORT Context2D {
 public:
  Context2D(GrGLInterface* interface);
  virtual ~Context2D();
  
  virtual void flush() = 0;
  virtual void finish() = 0;
  virtual Canvas2D* CreateCanvas(int32 width, int32 height) = 0;
 protected:
  DISALLOW_COPY_AND_ASSIGN(Context2D);
};

typedef std::shared_ptr<Context2D> Context2DPtr;

}  // namespace azer
