#pragma once

#include <memory>

#include "base/memory/ref_counted.h"
#include "ui/base/win/scoped_ole_initializer.h"

namespace base {
class AtExitManager;
class MessageLoop;
class MessageLoopForUI;
}

namespace azer {
class ChromiumEnv {
 public:
  ChromiumEnv(int argc, char* argv[]);
  ~ChromiumEnv();
 private:
  bool Init(int argc, char* argv[]);
  ::ui::ScopedOleInitializer ole_initializer_;
  ::std::unique_ptr< ::base::AtExitManager> exit_manager_;
  ::base::MessageLoop *message_loop_;
  DISALLOW_COPY_AND_ASSIGN(ChromiumEnv);
};
}  // namespace azer
