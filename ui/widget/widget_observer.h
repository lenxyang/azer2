#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace azer {
namespace widget {

class Widget;

class AZER_EXPORT WidgetObserver {
 public:
  virtual void OnWidgetObserving(Widget* widget) {}
  virtual void OnWidgetUnobserving(Widget* widget) {}
  virtual void OnWidgetPropertyChanged(Widget* widget, 
                                       const void* key, intptr_t old) {}
 protected:
  WidgetObserver() {}
  virtual ~WidgetObserver() {}
  DISALLOW_COPY_AND_ASSIGN(WidgetObserver);
};
}  // namespace widget
}  // namespace azer
