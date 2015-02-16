#pragma once

#include "base/memory/scoped_ptr.h"
#include "azer/ui/views/viewss_export.h"

namespace base {
class AtExitManager;
class MessageLoopForUI;
}

namespace wm {
class WMState;
}

namespace ui {
class ScopedOleInitializer;
}

namespace views {

class AuraScreen;

class VIEWS_EXPORT Env {
 public:
  Env(int argc, char* argv[]);
  virtual ~Env();
 private:
  scoped_ptr<AuraScreen> aura_screen_;
  scoped_ptr<ui::ScopedOleInitializer> ole_initializer_;
  scoped_ptr< ::base::AtExitManager> exit_manager_;
  scoped_ptr< ::base::MessageLoopForUI> message_loop_;
  DISALLOW_COPY_AND_ASSIGN(Env);
};
}  // namespace views
