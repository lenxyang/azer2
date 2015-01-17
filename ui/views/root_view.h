#pragma once

#include "azer/ui/views/view.h"

namespace azer {

namespace widget {
class WidgetTreeHost;
}

namespace views {
class AZER_EXPORT RootView : public View {
 public:
 private:
  scoped_ptr<WidgetTreeHost> host_;
  DISALLOW_COPY_AND_ASSIGN(View);
};
}  // namespace views
}  // namespace azer
