#pragma once

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "ui/events/event_source.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/geometry/rect.h"
#include "azer/base/export.h"

namespace ui {
class ViewProp;
}

namespace azer {
namespace win {


class Compositor;
class Window;
class WindowTreeHostObserver;
class WindowEventDispatcher;

class AZER_EXPORT WindowTreeHost {
 public:
  virtual ~WindowTreeHost();
  static WindowTreeHost* Create(const gfx::Rect& bounds);
  static WindowTreeHost* GetForAcceleratedWidget(gfx::AcceleratedWidget widget);

  void InitHost();

  void AddObserver(WindowTreeHostObserver* observer);
  void RemoveObserver(WindowTreeHostObserver* observer);

  Window* window() { return window_;}
  const Window* window() const { return window_;}

  WindowEventDispatcher* dispatcher() {
    return const_cast<WindowEventDispatcher*>(
        const_cast<const WindowTreeHost*>(this)->dispatcher());
  }
  const WindowEventDispatcher* dispatcher() const { return dispatcher_.get(); }
  
  // Cursor.
  // Sets the currently-displayed cursor. If the cursor was previously hidden
  // via ShowCursor(false), it will remain hidden until ShowCursor(true) is
  // called, at which point the cursor that was last set via SetCursor() will be
  // used.
  void SetCursor(gfx::NativeCursor cursor);

  // Invoked when the cursor's visibility has changed.
  void OnCursorVisibilityChanged(bool visible);

  // Moves the cursor to the specified location relative to the root window.
  void MoveCursorTo(const gfx::Point& location);

  // Moves the cursor to the |host_location| given in host coordinates.
  void MoveCursorToHostLocation(const gfx::Point& host_location);

  // Returns the EventSource responsible for dispatching events to the window
  // tree.
  virtual ui::EventSource* GetEventSource() = 0;

  // Returns the accelerated widget.
  virtual gfx::AcceleratedWidget GetAcceleratedWidget() = 0;

  virtual void Show() = 0;
  virtual void Hide() = 0;
  virtual gfx::Rect GetBounds() = 0;
  virtual void SetBounds(const gfx::Rect& rect) = 0;

  // Sets the OS capture to the root window.
  virtual void SetCapture() = 0;

  // Releases OS capture of the root window.
  virtual void ReleaseCapture() = 0;

  static gfx::Size GetNativeScreenSize();
 protected:
  WindowTreeHost();

  void DestroyCompositor();
  void DestroyDispatcher();
  void CreateCompositor(gfx::AcceleratedWidget accelerated_widget);

  // Returns the location of the RootWindow on native screen.
  virtual gfx::Point GetLocationOnNativeScreen() const = 0;


  void OnHostMoved(const gfx::Point& new_location);
  void OnHostResized(const gfx::Size& new_size);
  void OnHostCloseRequested();
  void OnHostActivated();
  void OnHostLostWindowCapture();

  // Sets the currently displayed cursor.
  virtual void SetCursorNative(gfx::NativeCursor cursor) = 0;

  // Moves the cursor to the specified location relative to the root window.
  virtual void MoveCursorToNative(const gfx::Point& location) = 0;

  // kCalled when the cursor visibility has changed.
  virtual void OnCursorVisibilityChangedNative(bool show) = 0;

  Window* window_;
  
  ObserverList<WindowTreeHostObserver> observers_;

  scoped_ptr<WindowEventDispatcher> dispatcher_;

  scoped_ptr<ui::ViewProp> prop_;
  DISALLOW_COPY_AND_ASSIGN(WindowTreeHost);
};
}  // namespace win
}  // namespace azer
