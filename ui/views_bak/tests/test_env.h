#pragma once

#include "base/memory/scoped_ptr.h"

namespace wm {
class WMState;
}

namespace ui {
class ScopedOleInitializer;
}

namespace views {
namespace test {
class TestEnv {
 public:
  TestEnv(int argc, char* argv[]);
  virtual ~TestEnv();
 private:
  scoped_ptr<ui::ScopedOleInitializer> ole_initializer_;
  DISALLOW_COPY_AND_ASSIGN(TestEnv);
};
}  // namespace test
}  // namespace views
