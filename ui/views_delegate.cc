#include "azer/ui/views_delegate.h"

#include "ui/wm/core/wm_state.h"
#include "ui/views/widget/widget.h"
#include "azer/ui/native_widget.h"
#include "azer/window_context.h"

namespace azer {
using namespace views;

DefaultViewsDelegate::DefaultViewsDelegate(WindowContext* ctx)
    : context_(ctx) {
}

DefaultViewsDelegate::~DefaultViewsDelegate() {
}

HICON DefaultViewsDelegate::GetSmallWindowIcon() const {
  return nullptr;
}

void DefaultViewsDelegate::OnBeforeWidgetInit(
    Widget::InitParams* params,
    internal::NativeWidgetDelegate* delegate) {
  if (!params->native_widget) {
    views::Widget* widget = dynamic_cast<views::Widget*>(delegate);
    views::NativeWidget* native_widget = NULL;
    switch (params->type) {
      case Widget::InitParams::TYPE_WINDOW:
        native_widget = adapter_->CreateWindowWidget(widget);
        break;
      case Widget::InitParams::TYPE_PANEL:
        native_widget = adapter_->CreatePanelWidget(widget);
        break;
      case Widget::InitParams::TYPE_CONTROL:
        native_widget = adapter_->CreateControlWidget(widget);
        break;
      case Widget::InitParams::TYPE_POPUP:
        native_widget = adapter_->CreatePopupWidget(widget);
        break;
      case Widget::InitParams::TYPE_MENU:
        native_widget = adapter_->CreateMenuWidget(widget);
        break;
      case Widget::InitParams::TYPE_TOOLTIP:
        native_widget = adapter_->CreateTooltipWidget(widget);
        break;
      case Widget::InitParams::TYPE_BUBBLE:
        native_widget = adapter_->CreateBubbleWidget(widget);
        break;
      case Widget::InitParams::TYPE_DRAG:
        native_widget = adapter_->CreateDragWidget(widget);
        break;
      default:
        native_widget = adapter_->CreateDesktopWidget(widget);
    }
    params->native_widget = native_widget;
  }
}

}  // namespace azer
