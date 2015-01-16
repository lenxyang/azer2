#pragma once

#include "base/compiler_specific.h"
#include "ui/events/event_constants.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/native_widget_types.h"
#include "azer/base/export.h"

namespace azer {
namespace widget {

class Widget;

class AZER_EXPORT WidgetDelegate : public ::ui::EventHandler {
 public:
  WidgetDelegate() {}
  virtual ~WidgetDelegate() {}
  virtual gfx::Size GetMinimumSize() const = 0;
  virtual gfx::Size GetMaximumSize() const = 0;

  virtual bool CanFocus() = 0;
  virtual bool OnCaptureLost() = 0;
  virtual void OnPaint(gfx::Canvas* canvas) = 0;

  // Returns true if event handling should descend into |child|. |location| is
  // in terms of the Widget.
  virtual bool ShouldDescendIntoChildForEventHandling(
      Widget* child,
      const gfx::Point& location) = 0;
};

}  // namespace widget
}  // namespace azer


