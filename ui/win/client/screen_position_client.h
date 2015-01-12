#include "azer/base/export.h"
#include "azer/ui/win/window.h"

namespace gfx {
class Display;
class Rect;
}

namespace azer {
namespace win {
class Window;

// An interface implemented by an object that changes coordinates within a root
// Window into system coordinates.
class AZER_EXPORT ScreenPositionClient {
public:
  virtual ~ScreenPositionClient() {}

  // Converts the |screen_point| from a given |window|'s coordinate space
  // into screen coordinate space.
  virtual void ConvertPointToScreen(const Window* window,
                                    gfx::Point* point) = 0;
  virtual void ConvertPointFromScreen(const Window* window,
                                      gfx::Point* point) = 0;
  // Converts the |screen_point| from root window host's coordinate of
  // into screen coordinate space.
  // A typical example of using this function instead of ConvertPointToScreen is
  // when X's native input is captured by a drag operation.
  // See the comments for ash::GetRootWindowRelativeToWindow for details.
  virtual void ConvertHostPointToScreen(Window* root_window,
                                        gfx::Point* point) = 0;
  // Sets the bounds of the window. The implementation is responsible
  // for finding out and translating the right coordinates for the |window|.
  virtual void SetBounds(Window* window,
                         const gfx::Rect& bounds,
                         const gfx::Display& display) = 0;
};

// Sets/Gets the activation client on the Window.
AZER_EXPORT void SetScreenPositionClient(Window* root_window,
                                         ScreenPositionClient* client);
AZER_EXPORT ScreenPositionClient* GetScreenPositionClient(
    const Window* root_window);

}  // namespace win
}  // namespace azer
