#pragma once

#include <atomic>
#include "azer/base/export.h"

namespace azer {
namespace widget {

class AZER_EXPORT WidgetContext {
 public:
  static void Init();
  static WidgetContext* GetInstance();
  static WidgetContext* Destroy();

  int64 allocate_widget_id();
 protected:
  WidgetContext();
  virtual ~WidgetContext();

  int64 allocated_widget_id_;
  static WidgetContext* inst_;
  DISALLOW_COPY_AND_ASSIGN(WidgetContext);
};

}  // namespace widget
}  // namespace azer

