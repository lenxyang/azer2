#pragma once

#include <atomic>
#include <string>

#include "base/observer_list.h"
#include "ui/base/cursor/cursor.h"
#include "ui/events/event_source.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/native_widget_types.h"

#include "azer/base/export.h"


namespace azer {

namespace compositor {
class Layer;
class LayerTreeHost;
class Compositor;
}  // namespace compositor

namespace widget {

class Widget;
class WidgetEventDispatcher;
class WidgetTreeHostObserver;

class AZER_EXPORT WidgetTreeHost {
 public:
  static WidgetTreeHost* Create(const gfx::Rect& bounds);

  static gfx::Size WidgetTreeHost::GetNativeScreenSize();

  virtual ~WidgetTreeHost();

  void AddObserver(WidgetTreeHostObserver* observer);
  void RemoveObserver(WidgetTreeHostObserver* observer);

  virtual gfx::AcceleratedWidget GetAcceleratedWidget() = 0;
  virtual void Show() = 0;
  virtual void Hide() = 0;
  virtual gfx::Rect GetBounds() = 0;
  virtual void SetBounds(const gfx::Rect& rect) = 0;
  virtual gfx::Point GetLocationOnNativeScreen() const  = 0;

  virtual ui::EventSource* GetEventSource() = 0;

  WidgetEventDispatcher* dispatcher();
  ::ui::EventProcessor* event_processor();

  Widget* root() { return root_;}
  const Widget* root() const { return root_;}

  compositor::Compositor* compositor();
  bool Closed() { return closed_;}

  // Sets the OS capture to the root window.
  virtual void SetCapture() = 0;

  // Releases OS capture of the root window.
  virtual void ReleaseCapture() = 0;

  void SetCursor(gfx::NativeCursor cursor);
 protected:
  WidgetTreeHost(const gfx::Rect& rect);

  void SetClosed(bool closed) { closed_ = closed;}
  void CreateCompositor(gfx::AcceleratedWidget widget);

  void OnHostMoved(const gfx::Point& new_location);
  void OnHostResized(const gfx::Size& new_size);
  void OnHostCloseRequested();
  void OnHostActivated();
  void OnHostLostWindowCapture();

  // Sets the currently displayed cursor.
  virtual void SetCursorNative(gfx::NativeCursor cursor) = 0;

  Widget* root_;
  std::atomic<bool> closed_;
  gfx::Rect bounds_;
  gfx::NativeCursor last_cursor_;
  scoped_ptr<WidgetEventDispatcher> dispatcher_;
  scoped_ptr<compositor::Compositor> compositor_;
  scoped_ptr<compositor::LayerTreeHost> layer_host_;
  ObserverList<WidgetTreeHostObserver> observers_;
  DISALLOW_COPY_AND_ASSIGN(WidgetTreeHost);
};

}  // namespace widget
}  // namespace azer
