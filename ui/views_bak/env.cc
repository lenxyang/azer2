#include "azer/ui/views/env.h"

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/i18n/icu_util.h"
#include "base/message_loop/message_loop.h"
#include "base/path_service.h"
#include "base/run_loop.h"
#include "ui/base/ime/input_method_initializer.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/win/scoped_ole_initializer.h"
#include "ui/base/ui_base_paths.h"
#include "ui/gfx/screen.h"
#include "ui/wm/core/wm_state.h"
#include "ui/gfx/win/dpi.h"

#include "azer/ui/aura/env.h"
#include "azer/ui/views/aura/aura_screen.h"
#include "azer/render/util/render_system_loader.h"

namespace views {

Env::Env(int argc, char* argv[]) {
#if defined(OS_WIN)
  gfx::InitDeviceScaleFactor(1.0f);
#endif
  ole_initializer_.reset(new ui::ScopedOleInitializer);
  exit_manager_.reset(new base::AtExitManager);
  CommandLine::Init(argc, argv);
  base::i18n::InitializeICU();
  CHECK(azer::LoadRenderSystem());

  aura::Env::CreateInstance(true);
  aura_screen_.reset(AuraScreen::Create(gfx::Size()));
  gfx::Screen::SetScreenInstance(gfx::SCREEN_TYPE_NATIVE, aura_screen_.get());
  message_loop_.reset(new ::base::MessageLoopForUI);

  ui::RegisterPathProvider();
  base::FilePath ui_test_pak_path;
  DCHECK(PathService::Get(ui::UI_TEST_PAK, &ui_test_pak_path));
  ui::ResourceBundle::InitSharedInstanceWithPakPath(ui_test_pak_path);
}

Env::~Env() {
}

}  // namespace views
