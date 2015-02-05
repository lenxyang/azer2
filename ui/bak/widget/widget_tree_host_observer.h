#pragma once

#include "azer/base/export.h"

namespace azer {
namespace widget {

class WidgetTreeHost;

class WidgetTreeHostObserver {
 public:
  virtual void OnDestroying(WidgetTreeHost* host) {}
  virtual void OnDestroyed(WidgetTreeHost* host) {}
 protected:
  WidgetTreeHostObserver() {}
  virtual ~WidgetTreeHostObserver() {}
  DISALLOW_COPY_AND_ASSIGN(WidgetTreeHostObserver);
};
}  // namespace widget
}  // namespace azer
