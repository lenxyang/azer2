#include "azer/ui/adapter/desktop_views_delegate.h"

#include "azer/ui/adapter/native_widget.h"

using namespace views;

namespace azer {

DesktopTestViewsDelegate::DesktopTestViewsDelegate() {}

DesktopTestViewsDelegate::~DesktopTestViewsDelegate() {}

void DesktopTestViewsDelegate::OnBeforeWidgetInit(
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
