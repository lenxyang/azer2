#pragma once

#include "azer/ui/widget/widget.h"

namespace gfx {
class Display;
class Rect;
}

namespace azer {
namespace widget {
class Widget;
namespace client {

// An interface implemented by an object that changes coordinates within a root
// Widget into system coordinates.
class AZER_EXPORT ScreenPositionClient {
 public:
  virtual ~ScreenPositionClient() {}

  // Converts the |screen_point| from a given |window|'s coordinate space
  // into screen coordinate space.
  virtual void ConvertPointToScreen(const Widget* window,
                                    gfx::Point* point) = 0;
  virtual void ConvertPointFromScreen(const Widget* window,
                                      gfx::Point* point) = 0;
  // Converts the |screen_point| from root window host's coordinate of
  // into screen coordinate space.
  // A typical example of using this function instead of ConvertPointToScreen is
  // when X's native input is captured by a drag operation.
  // See the comments for ash::GetRootWidgetRelativeToWidget for details.
  virtual void ConvertHostPointToScreen(Widget* root_window,
                                        gfx::Point* point) = 0;
  // Sets the bounds of the window. The implementation is responsible
  // for finding out and translating the right coordinates for the |window|.
  virtual void SetBounds(Widget* window,
                         const gfx::Rect& bounds,
                         const gfx::Display& display) = 0;
};

// Sets/Gets the activation client on the Widget.
AZER_EXPORT void SetScreenPositionClient(Widget* root_window,
                                         ScreenPositionClient* client);
AZER_EXPORT ScreenPositionClient* GetScreenPositionClient(
    const Widget* root_window);

}  // namespace clients
}  // namespace widget
}  // namespace azer

