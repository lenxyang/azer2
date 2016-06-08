#pragma once

#include "ui/views/widget/native_widget_aura.h"
#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"

namespace views {
namespace internal {
class NativeWidgetDelegate;
}
}

namespace azer {
class WindowContext;
class DesktopNativeWidget: public views::DesktopNativeWidgetAura {
 public:
  explicit DesktopNativeWidget(views::internal::NativeWidgetDelegate* delegate,
                               WindowContext* ctx);
 protected:
  ui::NativeTheme* GetNativeTheme() const override;
  WindowContext* context_;
  DISALLOW_COPY_AND_ASSIGN(DesktopNativeWidget);
};

class NativeWidget : public views::NativeWidgetAura {
 public:
  explicit NativeWidget(views::internal::NativeWidgetDelegate* delegate,
                        WindowContext* ctx);
 protected:
  ui::NativeTheme* GetNativeTheme() const override;
  WindowContext* context_;
  DISALLOW_COPY_AND_ASSIGN(NativeWidget);
};
}  // namespace azer
