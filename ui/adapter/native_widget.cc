#include "azer/ui/adapter/native_widget.h"

#include "azer/ui/theme/theme.h"
#include "ui/native_theme/native_theme_aura.h"

namespace azer {
DesktopNativeWidget::DesktopNativeWidget(
    views::internal::NativeWidgetDelegate* delegate)
    : DesktopNativeWidgetAura(delegate) {
}

ui::NativeTheme* DesktopNativeWidget::GetNativeTheme() const {
  return ui::NativeThemeAura::instance();
}

NativeWidget::NativeWidget(views::internal::NativeWidgetDelegate* delegate)
    : NativeWidgetAura(delegate) {
}

ui::NativeTheme* NativeWidget::GetNativeTheme() const {
  return ui::NativeThemeAura::instance();
}
}  // namespace azer
