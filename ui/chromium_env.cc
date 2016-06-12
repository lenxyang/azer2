#include "azer/ui/chromium_env.h"

#include "base/at_exit.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/command_line.h"
#include "base/location.h"
#include "base/i18n/icu_util.h"
#include "base/message_loop/message_loop.h"
#include "base/path_service.h"
#include "base/run_loop.h"
#include "components/tracing/trace_to_console.h"
#include "components/tracing/trace_config_file.h"
#include "components/tracing/tracing_switches.h"
#include "ui/base/ime/input_method_initializer.h"
#include "ui/base/material_design/material_design_controller.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/ui_base_paths.h"
#include "ui/views/widget/widget.h"

namespace azer {
ChromiumEnv::ChromiumEnv(int argc, char* argv[]) {
  CHECK(Init(argc, argv));
}

ChromiumEnv::~ChromiumEnv() {
  delete message_loop_;
  ui::ResourceBundle::CleanupSharedInstance();
  ui::ShutdownInputMethod();
}

bool ChromiumEnv::Init(int argc, char* argv[]) {
  ::base::CommandLine::Init(argc, argv);
  exit_manager_.reset(new base::AtExitManager);
  base::i18n::InitializeICU();
  ui::RegisterPathProvider();

  message_loop_ = new ::base::MessageLoopForUI;

  ui::MaterialDesignController::Initialize();
  base::FilePath ui_test_pak_path;
  DCHECK(PathService::Get(ui::UI_TEST_PAK, &ui_test_pak_path));
  ui::ResourceBundle::InitSharedInstanceWithPakPath(ui_test_pak_path);
  ui::InitializeInputMethodForTesting();

  // Start startup tracing through TracingController's interface. TraceLog has
  // been enabled in content_main_runner where threads are not available. Now We
  // need to start tracing for all other tracing agents, which require threads.
  base::CommandLine* cmdline = base::CommandLine::ForCurrentProcess();
  if (cmdline->HasSwitch(switches::kTraceToConsole)) {
      base::trace_event::TraceLog::GetInstance()->SetEnabled(
      tracing::GetConfigForTraceToConsole(),
        base::trace_event::TraceLog::RECORDING_MODE);
  }

  return true;
}
}  // namespace azer
