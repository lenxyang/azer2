#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/nine_image_painter_factory.h"
#include "azer/base/export.h"

namespace gfx {
class Canvas;
class ImageSkia;
class Insets;
class Rect;
class Size;
}

namespace azer {
namespace views {

class View;

class AZER_EXPORT Painter {
 public:
  Painter();
  virtual ~Painter();

  // Paints the painter in the specified region.
  virtual void Paint(gfx::Canvas* canvas, const gfx::Size& size) = 0;
 private:
  SkColor color_;
  DISALLOW_COPY_AND_ASSIGN(Painter);
};
}  // namespace views
}  // namespace azer
