#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace azer {
namespace win {

class WindowTreeHost;

class WinTestBase : public testing::Test {
 public:
  WinTestBase();
  ~WinTestBase() override;

  void SetUp() override;
  void TearDown() override;
 private:
  base::MessageLoopForUI message_loop_;
  scoped_ptr<WindowTreeHost> host_;
  DISALLOW_COPY_AND_ASSIGN(WinTestBase);
};
}  // namespace win
}  // namespace azer
