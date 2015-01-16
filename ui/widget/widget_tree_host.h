#pragma once

#include <atomic>
#include <string>

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

class AZER_EXPORT WidgetTreeHost {
 public:
  static WidgetTreeHost* Create(const gfx::Rect& bounds);

  static gfx::Size WidgetTreeHost::GetNativeScreenSize();

  virtual ~WidgetTreeHost();

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
 protected:
  WidgetTreeHost(const gfx::Rect& rect);

  void SetClosed(bool closed) { closed_ = closed;}
  void CreateCompositor(gfx::AcceleratedWidget widget);

  Widget* root_;
  std::atomic<bool> closed_;
  gfx::Rect bounds_;
  scoped_ptr<WidgetEventDispatcher> dispatcher_;
  scoped_ptr<compositor::Compositor> compositor_;
  scoped_ptr<compositor::LayerTreeHost> layer_host_;
  DISALLOW_COPY_AND_ASSIGN(WidgetTreeHost);
};

}  // namespace widget
}  // namespace azer
