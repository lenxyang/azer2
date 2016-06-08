#include "azer/ui/native_widget.h"

#include "base/logging.h"
#include "ui/native_theme/native_theme_aura.h"

#include "azer/ui/context.h"

namespace nelf {
DesktopNativeWidget::DesktopNativeWidget(
    views::internal::NativeWidgetDelegate* delegate, WindowContext* ctx)
    : views::DesktopNativeWidgetAura(delegate),
      context_(ctx) {
  DCHECK(adapter);
}

ui::NativeTheme* DesktopNativeWidget::GetNativeTheme() const {
  // return views::DesktopNativeWidgetAura::GetNativeTheme();
  return adapter_->GetNativeTheme();
}

// class NativeWidget
NativeWidget::NativeWidget(
    views::internal::NativeWidgetDelegate* delegate, WindowContext* ctx)
    : views::NativeWidgetAura(delegate),
      context_(ctx) {
  DCHECK(adapter);
}

ui::NativeTheme* NativeWidget::GetNativeTheme() const {
  // return views::NativeWidgetAura::GetNativeTheme();
  return adapter_->GetNativeTheme();
}
}  // namespace nelf
