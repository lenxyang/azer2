#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "testing/gtest/include/gtest/gtest.h"

#include "azer/ui/win/window_tree_host.h"
#include "azer/ui/win/context.h"

namespace azer {
namespace win {

class WindowTreeHost;

class WinTestBase : public testing::Test {
 public:
  WinTestBase();
  ~WinTestBase() override;

  void SetUp() override;
  void TearDown() override;
 protected:
  Window* root_window();
  void RunAllPendingInMessageLoop();
  bool DispatchEventUsingWindowDispatcher(::ui::Event* event);
  ui::EventProcessor* event_processor() { return host_->event_processor(); }

  base::MessageLoopForUI message_loop_;
  scoped_ptr<WindowTreeHost> host_;
  DISALLOW_COPY_AND_ASSIGN(WinTestBase);
};
}  // namespace win
}  // namespace azer
