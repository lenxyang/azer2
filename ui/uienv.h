#pragma once

#include <memory>
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/ui/render_loop.h"
#include "azer/ui/adapter/output_surface.h"
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
class NativeWidget;
class WidgetDelegate;
class ViewsDelegate;
}

namespace azer {
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
    views::NativeWidget* native_widget;
    Params()
        : width(800)
        , height(600)
        , fullscreen(false) 
        , view_delegate(NULL)
        , render_delegate(NULL)
        , native_widget(NULL) {
    }
  };
  bool MainLoop(views::Widget* widget, RenderLoop::Delegate* delegate);
 private:
  ui::ScopedOleInitializer ole_initializer_;
  std::unique_ptr< ::base::AtExitManager> exit_manager_;
  std::unique_ptr< ::base::MessageLoopForUI> message_loop_;

  std::unique_ptr<gfx::Screen> desktop_screen_;
  std::unique_ptr<wm::WMState> wm_state_;
  std::unique_ptr<UIContextFactory> context_factory_;
  std::unique_ptr<ViewsDelegate> views_delegate_;
  scoped_refptr<RenderLoop> render_loop_;
  DISALLOW_COPY_AND_ASSIGN(UIEnvironment);
};

}  // namespace azer
