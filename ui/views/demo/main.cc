
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "azer/ui/base/environment.h"
#include "azer/ui/views/view.h"
#include "azer/ui/views/root_view.h"

scoped_ptr<azer::UIEnv> uienv;

int main(int argc, char* argv[]) {
  uienv.reset(new azer::UIEnv(argc, argv));

  azer::widget::WidgetContext::Init();
  azer::views::RootView root(gfx::Rect(100, 100, 800, 600));
  root->Show();

  azer::widget::WidgetContext::Destroy();
  return 0;
}

