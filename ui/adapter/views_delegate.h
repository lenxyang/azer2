#pragma once

#include "base/memory/scoped_ptr.h"
#include "ui/views/views_delegate.h"
#include "azer/base/export.h"

namespace wm {
class WMState;
}

namespace azer {

class AZER_EXPORT DefaultViewsDelegate : public views::ViewsDelegate {
 public:
  DefaultViewsDelegate();
  ~DefaultViewsDelegate() override;

  // ViewsDelegate:
  void OnBeforeWidgetInit(views::Widget::InitParams* params,
                          views::internal::NativeWidgetDelegate* delegate) override;

 private:

  scoped_ptr<wm::WMState> wm_state_;

  ui::ContextFactory* context_factory_;
  DISALLOW_COPY_AND_ASSIGN(ViewsDelegate);
};

}  // namespace azer
