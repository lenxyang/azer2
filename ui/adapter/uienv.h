#pragma once

#include <memory>
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/base/render_loop.h"
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
class RenderLoop;
class UIContextFactory;

class AZER_EXPORT UIEnvironment {
public:
  UIEnvironment();
  ~UIEnvironment();

  RenderLoop* GetRenderLoop();

  bool Init(int argc, char* argv[]);

  // create main window and run loop
  struct Params {
    int32 width;
    int32 height;
    bool fullscreen;
    views::WidgetDelegate* view_delegate;
    RenderLoop::Delegate* render_delegate;

    Params()
        : width(800)
        , height(600)
        , fullscreen(false) 
        , view_delegate(NULL)
        , render_delegate(NULL) {
    }
  };
  bool MainLoop(const Params& params);
 private:
  ui::ScopedOleInitializer ole_initializer_;
  std::unique_ptr< ::base::AtExitManager> exit_manager_;
  std::unique_ptr< ::base::MessageLoopForUI> message_loop_;

  std::unique_ptr<gfx::Screen> desktop_screen_;
  std::unique_ptr<wm::WMState> wm_state_;
  std::unique_ptr<UIContextFactory> context_factory_;
  std::unique_ptr<DesktopTestViewsDelegate> desktop_views_delegate_;
  scoped_refptr<RenderLoop> render_loop_;
  DISALLOW_COPY_AND_ASSIGN(UIEnvironment);
};
}  // namespace azer
