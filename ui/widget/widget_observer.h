#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace azer {
namespace widget {

class Widget;

class AZER_EXPORT WidgetObserver {
 public:
  virtual void OnObserving(Widget* widget) {}
  virtual void OnUnobserving(Widget* widget) {}
  virtual void OnPropertyChanged(Widget* widget, const void* key, intptr_t old) {}
 protected:
  WidgetObserver() {}
  virtual ~WidgetObserver() {}
  DISALLOW_COPY_AND_ASSIGN(WidgetObserver);
};
}  // namespace widget
}  // namespace azer
