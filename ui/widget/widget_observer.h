#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace gfx {
class Rect;
}

namespace azer {
namespace widget {

class Widget;

class AZER_EXPORT WidgetObserver {
 public:
  virtual void OnWidgetAddedToRootWidget(Widget* widget) {}
  virtual void OnWidgetRemovingFromRootWidget(Widget* widget) {}
  virtual void OnWidgetObserving(Widget* widget) {}
  virtual void OnWidgetUnobserving(Widget* widget) {}
  virtual void OnWidgetPropertyChanged(Widget* widget, 
                                       const void* key, intptr_t old) {}
  virtual void OnWidgetStackingChanged(Widget* widget) {}

  virtual void OnWidgetVisibilityChanging(Widget* widget, bool visible) {}
  virtual void OnWidgetVisibilityChanged(Widget* widget, bool visible) {}
  virtual void OnWidgetBoundsChanged(Widget* widget,
                                     const gfx::Rect& old_bounds,
                                     const gfx::Rect& new_bounds) {}

  virtual void OnWidgetDestroying(Widget* widget) {}
  virtual void OnWidgetDestroyed(Widget* widget) {}
 protected:
  WidgetObserver() {}
  virtual ~WidgetObserver() {}
  DISALLOW_COPY_AND_ASSIGN(WidgetObserver);
};
}  // namespace widget
}  // namespace azer
