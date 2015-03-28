#include "azer/ui/uienv.h"

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/i18n/icu_util.h"
#include "base/message_loop/message_loop.h"
#include "base/path_service.h"
#include "base/run_loop.h"
#include "ui/aura/env.h"
#include "ui/aura/window.h"
#include "ui/base/ime/input_method_initializer.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/ui_base_paths.h"
#include "ui/gfx/screen.h"
#include "ui/wm/core/wm_state.h"
#include "ui/views/widget/desktop_aura/desktop_screen.h"

#include "azer/render/util/render_system_loader.h"
#include "azer/ui/widget_util.h"
#include "azer/ui/render_loop.h"
#include "azer/ui/adapter/context_factory.h"
#include "azer/ui/adapter/views_delegate.h"

namespace azer {

UIEnvironment::UIEnvironment() {
}

UIEnvironment::~UIEnvironment() {
  ui::ResourceBundle::CleanupSharedInstance();
  ui::ShutdownInputMethod();
  aura::Env::DeleteInstance();

  wm_state_.reset(NULL);
  views_delegate_.reset(NULL);
}

bool UIEnvironment::Init(int argc, char* argv[]) {
  exit_manager_.reset(new base::AtExitManager);
  CommandLine::Init(argc, argv);
  base::i18n::InitializeICU();
  message_loop_.reset(new ::base::MessageLoopForUI);

  CHECK(azer::LoadRenderSystem());

  ui::RegisterPathProvider();
  base::FilePath ui_test_pak_path;
  DCHECK(PathService::Get(ui::UI_TEST_PAK, &ui_test_pak_path));
  ui::ResourceBundle::InitSharedInstanceWithPakPath(ui_test_pak_path);
  ui::InitializeInputMethodForTesting();

  context_factory_.reset(new UIContextFactory);
  views_delegate_.reset(new DefaultViewsDelegate);
  aura::Env::CreateInstance(true);
  aura::Env::GetInstance()->set_context_factory(context_factory_.get());
  
  wm_state_.reset(new wm::WMState);
  desktop_screen_.reset(views::CreateDesktopScreen());
  gfx::Screen::SetScreenInstance(gfx::SCREEN_TYPE_NATIVE, desktop_screen_.get());

  return true;
}

bool UIEnvironment::MainLoop(views::Widget* widget, 
                             RenderLoop::Delegate* delegate) {
  CHECK(NULL != delegate);
  CHECK(NULL != widget);
  SetWidgetRendererWindow(widget);
  widget->Show();
  render_loop_ = new RenderLoop(delegate, widget);
  return render_loop_->Run();
}

RenderLoop* UIEnvironment::GetRenderLoop() {
  DCHECK(NULL != render_loop_);
  return render_loop_.get();
}

}  // namespace azer

