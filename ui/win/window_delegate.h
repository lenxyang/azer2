#pragma once

#include "base/compiler_specific.h"
#include "ui/events/event_constants.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/native_widget_types.h"
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
class AZER_EXPORT WindowDelegate : public ::ui::EventHandler {
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
