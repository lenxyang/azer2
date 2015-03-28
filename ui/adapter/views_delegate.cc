#include "azer/ui/adapter/views_delegate.h"

#include "ui/wm/core/wm_state.h"
#include "azer/ui/adapter/native_widget.h"

namespace azer {
using namespace views;

DefaultViewsDelegate::DefaultViewsDelegate()
    : use_desktop_native_widgets_(false),
      use_transparent_windows_(false) {
  DCHECK(!ViewsDelegate::views_delegate);
  ViewsDelegate::views_delegate = this;
  wm_state_.reset(new wm::WMState);
}

DefaultViewsDelegate::~DefaultViewsDelegate() {
  if (ViewsDelegate::views_delegate == this)
    ViewsDelegate::views_delegate = NULL;
}

void DefaultViewsDelegate::OnBeforeWidgetInit(
    Widget::InitParams* params,
    internal::NativeWidgetDelegate* delegate) {
  // If we already have a native_widget, we don't have to try to come
  // up with one.
  if (params->native_widget)
    return;

  if (params->parent &&
      params->type != views::Widget::InitParams::TYPE_MENU &&
      params->type != views::Widget::InitParams::TYPE_TOOLTIP) {
    params->native_widget = new NativeWidget(delegate);
  } else {
    params->native_widget = new DesktopNativeWidget(delegate);
  }
}

}  // namespace azer
