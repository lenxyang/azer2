#pragma once

#include "base/basictypes.h"
#include "ui/gfx/geometry/rect.h"
#include "azer/base/export.h"

namespace azer {
namespace win {

class Window;

class AZER_EXPORT WindowObserver {
 public:
  WindowObserver();
  virtual void OnWindowAdded(Window* new_win) {}
  virtual void OnWillRemoveWindow(Window* window) {}
  
  // Invoked when SetVisible() is invoked on a window. |visible| is the
  // value supplied to SetVisible(). If |visible| is true, window->IsVisible()
  // may still return false. See description in Window::IsVisible() for details.
  virtual void OnWindowVisibilityChanging(Window* window, bool visible) {}
  virtual void OnWindowVisibilityChanged(Window* window, bool visible) {}

  virtual void OnWindowBoundsChanged(Window* window,
                                     const gfx::Rect& old_bounds,
                                     const gfx::Rect& new_bounds) {}
  virtual void OnWindowStackingChanged(Window* window) {}
  virtual void OnWindowDestroying(Window* window) {}
  virtual void OnWindowDestroyed(Window* window) {}

  virtual void OnWindowTitleChanged(Window* window) {}

  // Invoked when SetProperty(), ClearProperty(), or
  // NativeWidgetAura::SetNativeWindowProperty() is called on the window.
  // |key| is either a WindowProperty<T>* (SetProperty, ClearProperty)
  // or a const char* (SetNativeWindowProperty). Either way, it can simply be
  // compared for equality with the property constant. |old| is the old property
  // value, which must be cast to the appropriate type before use.
  virtual void OnWindowPropertyChanged(Window* window,
                                       const void* key,
                                       intptr_t old) {}
  
protected:
  virtual ~WindowObserver();
private:
  friend class Window;

  void OnObservingWindow(Window* window); 
  void OnUnobservingWindow(Window* window); 
  int observing_;
  DISALLOW_COPY_AND_ASSIGN(WindowObserver);
};
}  // namespace win
}  // namespace azer
