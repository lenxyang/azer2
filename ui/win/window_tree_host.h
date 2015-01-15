#pragma once

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "ui/events/event_source.h"
#include "ui/events/event_processor.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/geometry/rect.h"
#include "azer/base/export.h"
#include "azer/ui/compositor/compositor.h"
#include "azer/ui/compositor/layer_tree_host.h"

namespace ui {
class ViewProp;
}

namespace azer {

namespace compositor {
class Layer;
class LayerTreeHost;
}

namespace win {
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
  
  ::ui::EventProcessor* event_processor();

  WindowEventDispatcher* dispatcher() {
    return const_cast<WindowEventDispatcher*>(
        const_cast<const WindowTreeHost*>(this)->dispatcher());
  }
  const WindowEventDispatcher* dispatcher() const { return dispatcher_.get(); }
  
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

  static gfx::Size GetNativeScreenSize();
 protected:
  WindowTreeHost();

  void DestroyCompositor();
  void DestroyDispatcher();
  void CreateCompositor(gfx::AcceleratedWidget accelerated_widget);

  // Returns the location of the RootWindow on native screen.
  virtual gfx::Point GetLocationOnNativeScreen() const = 0;

  Window* window_;
  
  ObserverList<WindowTreeHostObserver> observers_;

  scoped_ptr<WindowEventDispatcher> dispatcher_;

  scoped_ptr<ui::ViewProp> prop_;
  DISALLOW_COPY_AND_ASSIGN(WindowTreeHost);
};
}  // namespace win
}  // namespace azer
