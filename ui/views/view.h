#pragma once

#include "azer/base/export.h"
#include "azer/ui/widget/widget_delegate.h"

namespace azer {

namespace widget {
class Widget;
}  // namespace widget

namespace views {
class AZER_EXPORT View : public widget::WidgetDelegate {
 public:
 private:
  DISALLOW_COPY_AND_ASSIGN(View);
};
}  // namespace views
}  // namespace azer
