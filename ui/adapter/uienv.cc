#include "azer/ui/adapter/uienv.h"

#include "base/command_line.h"
#include "base/i18n/icu_util.h"
#include "base/path_service.h"
#include "base/run_loop.h"
#include "ui/aura/window.h"
#include "ui/aura/env.h"
#include "ui/base/ime/input_method_initializer.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/ui_base_paths.h"

#include "azer/ui/adapter/context_factory.h"

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
  context_factory_.reset(new UIContextFactory);
  message_loop_.reset(new ::base::MessageLoopForUI);

  ui::RegisterPathProvider();
  base::FilePath ui_test_pak_path;
  DCHECK(PathService::Get(ui::UI_TEST_PAK, &ui_test_pak_path));
  ui::ResourceBundle::InitSharedInstanceWithPakPath(ui_test_pak_path);
  ui::InitializeInputMethodForTesting();

  views_delegate_.reset(new DesktopTestViewsDelegate);
  aura::Env::CreateInstance(true);
  aura::Env::GetInstance()->set_context_factory(context_factory_.get());
  
  wm_state_.reset(new wm::WMState);
  desktop_screen_.reset(views::CreateDesktopScreen());
  gfx::Screen::SetScreenInstance(gfx::SCREEN_TYPE_NATIVE, desktop_screen_.get());

  return true;
}

ScopedUIEnvironment::ScopedUIEnvironment(int argc, char* argv[]) {
  scoped_obj_.reset(new UIEnvironment());
  CHECK(scoped_obj_->Init(argc, argv));
}
ScopedUIEnvironment::~ScopedUIEnvironment() {
}

}  // namespace azer
