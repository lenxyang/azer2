#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/i18n/icu_util.h"
#include "base/message_loop/message_loop.h"

#include "azer/ui/win/win.h"

using azer::ui::WindowTreeHost;

int main(int argc, char* argv[]) {
  CommandLine::Init(argc, argv); 
  base::AtExitManager exit_manager;
  base::i18n::InitializeICU();
  base::MessageLoopForUI message_loop;

  WindowTreeHost* host = WindowTreeHost::Create(gfx::Rect(0, 0, 800, 600));
  base::MessageLoopForUI::current()->Run();
  return 0;
}
