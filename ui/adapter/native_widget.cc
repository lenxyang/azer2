#include "azer/ui/adapter/native_widget.h"

#include "azer/ui/theme/theme.h"

namespace azer {
DesktopNativeWidget::DesktopNativeWidget(
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
