#include "azer/ui/native_widget.h"

#include "base/logging.h"
#include "ui/native_theme/native_theme_aura.h"
#include "ui/views/widget/native_widget_delegate.h"

#include "azer/ui/window_context.h"

namespace azer {
DesktopNativeWidget::DesktopNativeWidget(
    views::internal::NativeWidgetDelegate* delegate, WindowContext* ctx)
    : views::DesktopNativeWidgetAura(delegate),
      context_(ctx) {
  DCHECK(context_);
}

ui::NativeTheme* DesktopNativeWidget::GetNativeTheme() const {
  // return views::DesktopNativeWidgetAura::GetNativeTheme();
  return context_->GetNativeTheme();
}

// class NativeWidget
NativeWidget::NativeWidget(
    views::internal::NativeWidgetDelegate* delegate, WindowContext* ctx)
    : views::NativeWidgetAura(delegate),
      context_(ctx) {
  DCHECK(context_);
}

ui::NativeTheme* NativeWidget::GetNativeTheme() const {
  // return views::NativeWidgetAura::GetNativeTheme();
  return context_->GetNativeTheme();
}
}  // namespace azer
