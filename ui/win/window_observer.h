#pragma once

#include "base/basictypes.h"

#include "azer/base/export.h"

namespace azer {
namespace win {

class Window;

class AZER_EXPORT WindowObserver {
 public:
  WindowObserver();
  virtual void OnWindowAdded(Window* new_win) {}
  virtual void OnWindowBoundsChanged(Window* window,
                                     const gfx::Rect& old_bounds,
                                     const gfx::Rect& new_bounds) {}
  virtual void OnWindowStackingChanged(Window* window) {}
  virtual void OnWindowDestroying(Window* window) {}
  virtual void OnWindowDestroyed(Window* window) {}

  virtual void OnWindowTitleChanged(Window* window) {}
protected:
  virtual ~WindowObserver();
private:
  friend class Window;

  void OnObservingWindow(Window* window); 
  void OnUnobservingWindow(Window* window); 
  int observing_;
  DISALLOW_COPY_AND_ASSIGN(WindowEventDispatcher);
};
}  // namespace win
}  // namespace azer
