#pragma once

#include <string>

#include "ui/events/event_source.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/point.h"

#include "azer/base/export.h"


namespace azer {

namespace compositor {
class Layer;
class LayerTreeHost;
class Compositor;
}  // namespace compositor

namespace widget {

class Widget;

class AZER_EXPORT WidgetTreeHost {
 public:
  static WidgetTreeHost* Create(const gfx::Rect& bounds);

  virtual ~WidgetTreeHost();

  virtual gfx::AcceleratedWidget GetAcceleratedWidget() = 0;
  virtual void Show() = 0;
  virtual void Hide() = 0;

  virtual ui::EventSource* GetEventSource() = 0;

  Widget* root() { return widget_;}
  const Widget* root() const { return widget_;}
 protected:
  WidgetTreeHost();

  void InitCompositor();

  Widget* root_;
  scoped_ptr<compositor::Compositor> compositor_;
  scoped_ptr<compositor::LayerTreeHost> compositor_;
  DISALLOW_COPY_AND_ASSIGN(WidgetTreeHost);
};

}  // namespace widget
}  // namespace azer
