#pragma once

#include "base/basictypes.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/geometry/rect.h"

#include "azer/base/export.h"

namespace azer {
namespace win {

class Compositor;

class AZER_EXPORT WindowTreeHost {
 public:
  virtual ~WindowTreeHost();
  static WindowTreeHost* Create(const gfx::Rect& bounds);
  static WindowTreeHost* GetForAcceleratedWidget(gfx::AcceleratedWidget widget);

  virtual void Show() = 0;
  virtual void Hide() = 0;
  virtual gfx::Rect GetBounds() = 0;
  virtual void SetBounds(const gfx::Rect& rect) = 0;
  virtual gfx::Point GetLocationOnNativeScreen() const = 0;

  // Sets the OS capture to the root window.
  virtual void SetCapture() = 0;

  // Releases OS capture of the root window.
  virtual void ReleaseCapture() = 0;

  static gfx::Size GetNativeScreenSize();
 protected:
  WindowTreeHost();

  void DestroyCompositor();
  void DestroyDispatcher();

  // Sets the currently displayed cursor.
  virtual void SetCursorNative(gfx::NativeCursor cursor) = 0;

  // Moves the cursor to the specified location relative to the root window.
  virtual void MoveCursorToNative(const gfx::Point& location) = 0;

  // kCalled when the cursor visibility has changed.
  virtual void OnCursorVisibilityChangedNative(bool show) = 0;
  DISALLOW_COPY_AND_ASSIGN(WindowTreeHost);
};
}  // namespace win
}  // namespace azer
