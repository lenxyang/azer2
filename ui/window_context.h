#pragma once

#include "ui/native_theme/native_theme.h"

namespace views {
class Widget;
class NativeWidget;
}

namespace azer {
class WindowContext {
 public:
  explicit WindowContext();
  virtual ~WindowContext();
  virtual bool Init(int argc, char* argv[]) = 0;

  virtual views::NativeWidget* CreateDesktopWidget(views::Widget* widget) = 0;
  virtual views::NativeWidget* CreateWidget(views::Widget* widget) = 0;

  // create widget for specified purpose
  virtual views::NativeWidget* CreateWindowWidget(views::Widget* widget) = 0;
  virtual views::NativeWidget* CreatePanelWidget(views::Widget* widget) = 0;
  virtual views::NativeWidget* CreatePopupWidget(views::Widget* widget) = 0;
  virtual views::NativeWidget* CreateControlWidget(views::Widget* widget) = 0;
  virtual views::NativeWidget* CreateDragWidget(views::Widget* widget) = 0;
  virtual views::NativeWidget* CreateBubbleWidget(views::Widget* widget) = 0;
  virtual views::NativeWidget* CreateMenuWidget(views::Widget* widget) = 0;
  virtual views::NativeWidget* CreateTooltipWidget(views::Widget* widget) = 0;
  virtual ui::NativeTheme* GetNativeTheme() = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(WindowContext);
};
}  // namespace azer
