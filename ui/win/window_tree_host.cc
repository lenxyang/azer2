#include "azer/ui/win/window_tree_host.h"

#include "ui/base/view_prop.h"

#include "azer/ui/win/window.h"
#include "azer/ui/win/window_tree_host_observer.h"
#include "azer/ui/win/window_event_dispatcher.h"

namespace azer {
namespace win {

WindowTreeHost::WindowTreeHost() {
}

WindowTreeHost::~WindowTreeHost() {
}

void WindowTreeHost::DestroyCompositor() {
}

void WindowTreeHost::DestroyDispatcher() {
}

void WindowTreeHost::CreateCompositor(gfx::AcceleratedWidget accelerated_widget) {
  dispatcher_.reset(new WindowEventDispatcher(this));
}

void WindowTreeHost::OnHostActivated() {
}

void WindowTreeHost::OnHostLostWindowCapture() {
}

void WindowTreeHost::AddObserver(WindowTreeHostObserver* observer) {
  observers_.AddObserver(observer);
}

void WindowTreeHost::RemoveObserver(WindowTreeHostObserver* observer) {
  observers_.RemoveObserver(observer);
}

}  // namespace win
}  // namespace azer
