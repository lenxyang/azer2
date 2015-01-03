#pragma once

#include "azer/base/export.h"
#include "azer/ui/adapter/context_factory.h"
#include "azer/ui/adapter/desktop_test_views_delegate.h"
#include "base/message_loop/message_loop.h"
#include "base/at_exit.h"
#include "ui/base/win/scoped_ole_initializer.h"
#include "ui/gfx/screen.h"
#include "ui/views/widget/desktop_aura/desktop_screen.h"
#include "ui/wm/core/wm_state.h"

namespace azer {

class UIContextFactory;

class AZER_EXPORT UIEnvironment {
 public:
  UIEnvironment();
  ~UIEnvironment();

  bool Init(int argc, char* argv[]);
 private:
  ui::ScopedOleInitializer ole_initializer_;
  scoped_ptr< ::base::AtExitManager> exit_manager_;
  scoped_ptr< ::base::MessageLoopForUI> message_loop_;

  scoped_ptr<gfx::Screen> desktop_screen_;
  scoped_ptr<wm::WMState> wm_state_;
  scoped_ptr<UIContextFactory> context_factory_;
  scoped_ptr<DesktopTestViewsDelegate> views_delegate_;

  DISALLOW_COPY_AND_ASSIGN(UIEnvironment);
};

class ScopedUIEnvironment {
 public:
  ScopedUIEnvironment(int argc, char* argv[]);
  ~ScopedUIEnvironment();
 private:
  scoped_ptr<UIEnvironment> scoped_obj_;
  DISALLOW_COPY_AND_ASSIGN(ScopedUIEnvironment);
};

}  // namespace azer
