#pragma once

#include "base/memory/scoped_ptr.h"
#include "ui/views/views_delegate.h"
#include "azer/base/export.h"

namespace wm {
class WMState;
}

namespace azer {

class AZER_EXPORT TestViewsDelegate : public views::ViewsDelegate {
 public:
  TestViewsDelegate();
  ~TestViewsDelegate() override;

  // If set to |true|, forces widgets that do not provide a native widget to use
  // DesktopNativeWidgetAura instead of whatever the default native widget would
  // be. This has no effect on ChromeOS.
  void set_use_desktop_native_widgets(bool desktop) {
    use_desktop_native_widgets_ = desktop;
  }

  void set_use_transparent_windows(bool transparent) {
    use_transparent_windows_ = transparent;
  }

  // ViewsDelegate:
  void OnBeforeWidgetInit(views::Widget::InitParams* params,
                          views::internal::NativeWidgetDelegate* delegate) override;

 private:
  bool use_desktop_native_widgets_;

  bool use_transparent_windows_;

  scoped_ptr<wm::WMState> wm_state_;

  ui::ContextFactory* context_factory_;

  DISALLOW_COPY_AND_ASSIGN(TestViewsDelegate);
};

}  // namespace azer
