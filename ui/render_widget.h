#pragma once

#include "ui/views/widget/root_view.h"
#include "ui/views/widget/widget.h"

namespace azer {
class RenderWindow;

// About KeyEvent
// when keyevent pressed, the width(not top level, won't get key down event)
// so we transfer the focus to RenderView it will get the message when focused.
// all mouse event the RenderWidget can get, so it should put all the message
// to RenderView
class RenderWindowWidget : public views::Widget {
 public:
  RenderWindowWidget(RenderWindow* window);
  views::internal::RootView* CreateRootView() override;
  void OnNativeFocus() override;
 private:
  RenderWindow* window_;
  DISALLOW_COPY_AND_ASSIGN(RenderWindowWidget);
};
}  // namespace azer
