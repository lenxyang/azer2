#pragma once

#include "azer/base/export.h"

namespace gfx {
class Canvas;
class Path;
class Point;
class Rect;
class Size;
}  // namespace gfx

namespace azer {
namespace win {
class AZER_EXPORT WindowDelegate {
public:
  virtual gfx::Size GetMinimumSize() const = 0;
  virtual gfx::Size GteMaximumSize() const = 0;

  virtual bool CanFocus() = 0;
  virtual bool OnCaptureLost() = 0;
  virtual void OnPaint(gfx::Canvas* canvas) = 0;
private:
  DISALLOW_COPY_AND_ASSIGN(WindowDelegate);
};
}  // namespace win
}  // namespace azer
