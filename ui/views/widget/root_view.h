#pragma once

#include <vector>

#include "azer/ui/views/view.h"

namespace views {
class Widget;

namespace internal {
class VIEWS_EXPORT RootView : public View {
 public:
  RootView(Widget* widget);
  virtual ~RootView();

  void Init(const gfx::Rect& bounds);
 private:
  Widget* widget_;

  friend class View;
  DISALLOW_COPY_AND_ASSIGN(RootView);
};
}  // namespace internal
}  // namespace views
