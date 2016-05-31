#pragma once

#include <memory>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {

class RenderSystem;
class Canvas2D;

class AZER_EXPORT Context2D : public ::base::RefCounted<Context2D> {
 public:
  Context2D();
  virtual ~Context2D();
  
  virtual void flush() = 0;
  virtual void finish() = 0;
  virtual Canvas2D* CreateCanvas(int32_t width, int32_t height) = 0;
 protected:
  DISALLOW_COPY_AND_ASSIGN(Context2D);
};

typedef scoped_refptr<Context2D> Context2DPtr;

}  // namespace azer
