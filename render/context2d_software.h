#pragma once

#include <memory>

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/context2d.h"

namespace azer {

class RenderSystem;
class Canvas2D;

class AZER_EXPORT SoftwareContext2D : public Context2D {
 public:
  SoftwareContext2D();
  virtual ~SoftwareContext2D();
  
  void flush() override;
  void finish() override;
  Canvas2D* CreateCanvas(int32 width, int32 height) override;
 protected:
  DISALLOW_COPY_AND_ASSIGN(SoftwareContext2D);
};

}  // namespace azer
