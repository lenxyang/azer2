#include "azer/ui/adapter/views_delegate.h"

#include "ui/wm/core/wm_state.h"
#include "azer/ui/views/widget/desktop_aura/desktop_native_widget_aura.h"

namespace azer {
using namespace views;

TestViewsDelegate::TestViewsDelegate()
    : use_desktop_native_widgets_(false),
      use_transparent_windows_(false) {
  DCHECK(!ViewsDelegate::views_delegate);
  ViewsDelegate::views_delegate = this;
  wm_state_.reset(new wm::WMState);
}

TestViewsDelegate::~TestViewsDelegate() {
  if (ViewsDelegate::views_delegate == this)
    ViewsDelegate::views_delegate = NULL;
}

void TestViewsDelegate::OnBeforeWidgetInit(
    Widget::InitParams* params,
    internal::NativeWidgetDelegate* delegate) {
  if (params->opacity == Widget::InitParams::INFER_OPACITY) {
    params->opacity = use_transparent_windows_ ?
        Widget::InitParams::TRANSLUCENT_WINDOW :
        Widget::InitParams::OPAQUE_WINDOW;
  }
  if (!params->native_widget && use_desktop_native_widgets_)
    params->native_widget = new DesktopNativeWidgetAura(delegate);
}

}  // namespace azer
