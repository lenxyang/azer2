#pragma once

#include <memory>
#include "azer/base/export.h"
#include "ui/base/win/scoped_ole_initializer.h"

namespace wm {
class WMState;
}

namespace gfx {
class Screen;
}

namespace base {
class MessageLoop;
class MessageLoopForUI;
class AtExitManager;
}

namespace views {
class Widget;
class WidgetDelegate;
}

namespace azer {

class DesktopTestViewsDelegate;
class UIContextFactory;

class AZER_EXPORT UIEnvironment {
 public:
  struct Options {
    int32 width;
    int32 height;
    bool fullscreen;

    Options()
        : width(800)
        , height(600)
        , fullscreen(false) {
    }
  };

  UIEnvironment(const Options& options);
  ~UIEnvironment();

  bool Init(int argc, char* argv[]);

  // create main window and run loop
  bool MainLoop(views::WidgetDelegate* delegate);
 private:
  ui::ScopedOleInitializer ole_initializer_;
  std::unique_ptr< ::base::AtExitManager> exit_manager_;
  std::unique_ptr< ::base::MessageLoopForUI> message_loop_;

  std::unique_ptr<gfx::Screen> desktop_screen_;
  std::unique_ptr<wm::WMState> wm_state_;
  std::unique_ptr<UIContextFactory> context_factory_;
  std::unique_ptr<DesktopTestViewsDelegate> views_delegate_;
  const Options options_;
  DISALLOW_COPY_AND_ASSIGN(UIEnvironment);
};

}  // namespace azer
