#pragma once

#include <vector>

#include "azer/ui/views/view.h"

namespace views {

class Widget;

class VIEWS_EXPORT RootView : public View {
 public:
  RootView(Widget* widget);
  virtual ~RootView();
 private:
  Widget* widget_;
  DISALLOW_COPY_AND_ASSIGN(RootView);
};
}  // namespace views
