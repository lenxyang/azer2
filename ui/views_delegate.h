#pragma once

#include "ui/views/views_delegate.h"
#include "azer/base/export.h"

namespace wm {
class WMState;
}

namespace azer {

class WindowContext;
class DefaultViewsDelegate : public views::ViewsDelegate {
 public:
  explicit DefaultViewsDelegate(WindowContext* context);
  ~DefaultViewsDelegate() override;

  // ViewsDelegate:
  HICON GetSmallWindowIcon() const override;
  void OnBeforeWidgetInit(views::Widget::InitParams* params,
                          views::internal::NativeWidgetDelegate* delegate) override;
 private:
  std::unique_ptr<wm::WMState> wm_state_;
  ui::ContextFactory* context_factory_;
  WindowContext* context_;
  DISALLOW_COPY_AND_ASSIGN(DefaultViewsDelegate);
};
}  // namespace azer


