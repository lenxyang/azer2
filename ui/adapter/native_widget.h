#pragma once

#include "ui/native_theme/native_theme.h"
#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"
#include "ui/views/widget/native_widget_aura.h"

namespace azer {
class DesktopNativeWidget : public views::DesktopNativeWidgetAura {
 public:
  DesktopNativeWidget(views::internal::NativeWidgetDelegate* delegate);
  ui::NativeTheme* GetNativeTheme() const override;
 private:
  DISALLOW_COPY_AND_ASSIGN(DesktopNativeWidget);
};

class NativeWidget : public views::NativeWidgetAura {
 public:
  NativeWidget(views::internal::NativeWidgetDelegate* delegate);
  ui::NativeTheme* GetNativeTheme() const override;
 private:
  DISALLOW_COPY_AND_ASSIGN(NativeWidget);
};
}  // namespace azer
