#pragma once

#include "base/basictypes.h"
#include "azer/ui/views/views_export.h"
#include "azer/ui/views/view.h"

namespace views {

class Widget;

class VIEWS_EXPORT RootView : public View {
 public:
  explicit RootView(Widget* widget);
  RootView() override;

  void SetBounds(const gfx::Rect& bounds);
 private:
  DISALLOW_COPY_AND_ASSIGN(RootView);
};
}  // namespace views
