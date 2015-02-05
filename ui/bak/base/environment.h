#pragma once

#include "base/memory/scoped_ptr.h"
#include "azer/base/export.h"
#include "ui/base/win/scoped_ole_initializer.h"

namespace base {
class AtExitManager;
class MessageLoopForUI;
}

namespace wm {
class WMState;
}

namespace azer {
class AZER_EXPORT UIEnv {
 public:
  UIEnv(int argc, char* argv[]);
  ~UIEnv();
 private:
  ui::ScopedOleInitializer ole_initializer_;
  scoped_ptr< ::base::AtExitManager> exit_manager_;
  scoped_ptr< ::base::MessageLoopForUI> message_loop_;
  DISALLOW_COPY_AND_ASSIGN(UIEnv);
};
}  // namespace azer
