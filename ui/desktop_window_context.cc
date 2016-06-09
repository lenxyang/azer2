#include "azer/ui/desktop_window_context.h"

#include "ui/aura/env.h"
#include "ui/aura/window.h"
#include "ui/display/display.h"
#include "ui/display/screen.h"
#include "ui/wm/core/wm_state.h"
#include "ui/views/widget/desktop_aura/desktop_screen.h"

#include "azer/render/render_system_loader.h"
#include "azer/ui/adapter/context_factory.h"
#include "azer/ui/native_widget.h"
#include "azer/ui/views_delegate.h"
#include "azer/ui/focus_manager_factory.h"

namespace azer {
DesktopWindowContext::DesktopWindowContext() {
}

DesktopWindowContext::~DesktopWindowContext() {
  wm_state_.reset(NULL);
  views_delegate_.reset(NULL);
}

bool DesktopWindowContext::Init(int argc, char* argv[]) {
  CHECK(azer::LoadRenderSystem());

  FocusManagerFactory::SetInstance(new FocusManagerFactory);

  wm_state_.reset(new wm::WMState);
  views_delegate_.reset(new DefaultViewsDelegate(this));
  context_factory_.reset(new azer::UIContextFactory);
  aura_env_ = std::move(aura::Env::CreateInstance());
  aura::Env::GetInstance()->set_context_factory(context_factory_.get());

  desktop_screen_.reset(views::CreateDesktopScreen());
  display::Screen::SetScreenInstance(desktop_screen_.get());

  return true;
}

views::NativeWidget* DesktopWindowContext::CreateDesktopWidget(views::Widget* widget) {
  return new DesktopNativeWidget(widget, this);
}

views::NativeWidget* DesktopWindowContext::CreateWidget(views::Widget* widget) {
  return NULL;
}

views::NativeWidget* DesktopWindowContext::CreateWindowWidget(views::Widget* widget)  {
  return CreateDesktopWidget(widget);
}

views::NativeWidget* DesktopWindowContext::CreatePanelWidget(views::Widget* widget)  {
  return CreateDesktopWidget(widget);
}

views::NativeWidget* DesktopWindowContext::CreatePopupWidget(views::Widget* widget)  {
  return CreateDesktopWidget(widget);
}

views::NativeWidget* DesktopWindowContext::CreateControlWidget(views::Widget* widget)  {
  return CreateDesktopWidget(widget);
}

views::NativeWidget* DesktopWindowContext::CreateTooltipWidget(views::Widget* widget) {
  return CreateDesktopWidget(widget);
}

views::NativeWidget* DesktopWindowContext::CreateDragWidget(views::Widget* widget) {
  return CreateDesktopWidget(widget);
}

views::NativeWidget* DesktopWindowContext::CreateBubbleWidget(views::Widget* widget) {
  return CreateDesktopWidget(widget);
}

views::NativeWidget* DesktopWindowContext::CreateMenuWidget(views::Widget* widget) {
  return CreateDesktopWidget(widget);
}
}  // namespace azer
