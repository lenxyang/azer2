#include "azer/ui/win/window_tree_host.h"

#include "ui/base/view_prop.h"

#include "azer/ui/win/window.h"
#include "azer/ui/win/window_targeter.h"
#include "azer/ui/win/window_tree_host_observer.h"
#include "azer/ui/win/window_event_dispatcher.h"

namespace azer {
namespace win {

const char kWindowTreeHostForAcceleratedWidget[] =
    "__AZER_WINDOW_TREE_HOST_ACCELERATED_WIDGET__";

WindowTreeHost::WindowTreeHost() 
    : window_(new Window(NULL)) {
}

WindowTreeHost::~WindowTreeHost() {
}

void WindowTreeHost::DestroyCompositor() {
}

void WindowTreeHost::DestroyDispatcher() {
}

void WindowTreeHost::CreateCompositor(gfx::AcceleratedWidget accelerated_widget) {
  window()->set_host(this);
  window()->SetName("RootWindow");
  window()->SetEventTargeter(scoped_ptr< ::ui::EventTargeter>(new WindowTargeter()));
  prop_.reset(new ::ui::ViewProp(GetAcceleratedWidget(),
                               kWindowTreeHostForAcceleratedWidget,
                               this));
  dispatcher_.reset(new WindowEventDispatcher(this));
}


void WindowTreeHost::AddObserver(WindowTreeHostObserver* observer) {
  observers_.AddObserver(observer);
}

void WindowTreeHost::RemoveObserver(WindowTreeHostObserver* observer) {
  observers_.RemoveObserver(observer);
}

::ui::EventProcessor* WindowTreeHost::event_processor() {
  return dispatcher();
}

}  // namespace win
}  // namespace azer
