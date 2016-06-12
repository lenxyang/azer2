#pragma once

#include <memory>
#include "ui/display/screen.h"
#include "azer/ui/window_context.h"

namespace aura {
class Env;
}

namespace wm {
class WMState;
}

namespace views {
class Widget;
class NativeWidget;
class WidgetDelegate;
class ViewsDelegate;
}

namespace ui {
class ContextFactory;
class NativeTheme;
}

namespace azer {
class DesktopWindowContext : public WindowContext {
 public:
  explicit DesktopWindowContext();
  ~DesktopWindowContext();
  bool Init(int argc, char* argv[]) override;

  views::NativeWidget* CreateDesktopWidget(views::Widget* widget) override;
  views::NativeWidget* CreateWidget(views::Widget* widget) override;
  views::NativeWidget* CreateWindowWidget(views::Widget* widget) override;
  views::NativeWidget* CreatePanelWidget(views::Widget* widget) override;
  views::NativeWidget* CreatePopupWidget(views::Widget* widget) override;
  views::NativeWidget* CreateControlWidget(views::Widget* widget) override;
  views::NativeWidget* CreateDragWidget(views::Widget* widget) override;
  views::NativeWidget* CreateBubbleWidget(views::Widget* widget) override;
  views::NativeWidget* CreateMenuWidget(views::Widget* widget) override;
  views::NativeWidget* CreateTooltipWidget(views::Widget* widget) override;
  ui::NativeTheme* GetNativeTheme() override;
 private:
  std::unique_ptr<aura::Env> aura_env_;
  std::unique_ptr<display::Screen> desktop_screen_;
  std::unique_ptr<wm::WMState> wm_state_;
  std::unique_ptr<ui::ContextFactory> context_factory_;
  std::unique_ptr<views::ViewsDelegate> views_delegate_;
  DISALLOW_COPY_AND_ASSIGN(DesktopWindowContext);
};

}  // namespace azer
