#pragma once


#include "azer/ui/adapter/views_delegate.h"

namespace azer {

// Most aura test code is written assuming a single RootWindow view, however,
// at higher levels like content_browsertests and
// views_examples_with_content_exe, we must use the Desktop variants.
class DesktopTestViewsDelegate : public TestViewsDelegate {
 public:
  DesktopTestViewsDelegate();
  ~DesktopTestViewsDelegate() override;

  // Overridden from ViewsDelegate:
  void OnBeforeWidgetInit(views::Widget::InitParams* params,
                          views::internal::NativeWidgetDelegate* delegate) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(DesktopTestViewsDelegate);
};


}  // namespace azer
