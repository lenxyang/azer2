#include "azer/uisbox/adapter/base/desktop_views_delegate.h"

#include "ui/views/widget/native_widget_aura.h"
#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"

using namespace views;

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
    params->native_widget = new views::NativeWidgetAura(delegate);
  } else {
    params->native_widget = new views::DesktopNativeWidgetAura(delegate);
  }
}

