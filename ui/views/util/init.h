#pragma once

#include "azer/ui/views/views_export.h"
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

namespace views {

class VIEWS_EXPORT ViewsInitialize {
 public:
  ViewsInitialize(int argc, char* argv[]);
  ~ViewsInitialize();
 private:
  scoped_ptr<aura::TestScreen> aura_screen_;
  ui::ScopedOleInitializer ole_initializer_;
  scoped_ptr< ::base::AtExitManager> exit_manager_;
  scoped_ptr< ::base::MessageLoopForUI> message_loop_;
  DISALLOW_COPY_AND_ASSIGN(ViewsInitialize);
};
}  // namespace views
