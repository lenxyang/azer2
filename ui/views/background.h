#pragma once

#if defined(OS_WIN)
#include <windows.h>
#endif  // defined(OS_WIN)

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"
#include "azer/base/export.h"

namespace gfx {
class Canvas;
}  // namespace gfx

namespace azer {
namespace views {

class Painter;

class AZER_EXPORT Background {
 public:
  Background();
  virtual ~Background();

  // Creates a Background from the specified Painter. If owns_painter is
  // true, the Painter is deleted when the Border is deleted.
  static Background* CreateBackgroundPainter(bool owns_painter,
                                             Painter* painter);

  virtual void Paint(gfx::Canvas* canvas, View* view) const = 0;
 private:
  SkColor color_;
  DISALLOW_COPY_AND_ASSIGN(Background);
};
}  // namespace views
}  // namespace azer
