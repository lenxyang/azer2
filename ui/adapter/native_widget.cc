#include "azer/ui/adapter/native_widget.h"

#include "azer/ui/theme/theme.h"

namespace azer {
DesktopNativeWidgetAura::DesktopNativeWidgetAura(
    views::internal::NativeWidgetDelegate* delegate)
    : DesktopNativeWidgetAura(delegate) {
}

ui::NativeTheme* DesktopNativeWidget::GetNativeTheme() const {
  return Theme::GetInstance();
}

NativeWidget::NativeWidget(views::internal::NativeWidgetDelegate* delegate)
    : NativeWidgetAura(delegate) {
}

ui::NativeTheme* NativeWidget::GetNativeTheme() const {
  return Theme::GetInstance();
}
}  // namespace azer
