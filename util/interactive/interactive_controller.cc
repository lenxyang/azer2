#include "azer/util/interactive/interactive_controller.h"

namespace azer {
InteractiveController::InteractiveController() {}
InteractiveController::~InteractiveController() {}

void InteractiveController::OnControllerPicked() {
  FOR_EACH_OBSERVER(InteractiveControllerObserver, observer_list_, 
                    OnControllerPicked(this));
}

void InteractiveController::OnControllerUnpicked() {
  FOR_EACH_OBSERVER(InteractiveControllerObserver, observer_list_, 
                    OnControllerUnpicked(this));
}

void InteractiveController::OnControllerChanged() {
  FOR_EACH_OBSERVER(InteractiveControllerObserver, observer_list_, 
                    OnControllerChanged(this));
}

void InteractiveController::AddObserver(InteractiveControllerObserver* obs) {
  observer_list_.AddObserver(obs);
}

void InteractiveController::Removebserver(InteractiveControllerObserver* obs) {
  observer_list_.RemoveObserver(obs);
}

bool InteractiveController::HasObserver(InteractiveControllerObserver* obs) {
  return observer_list_.HasObserver(obs);
}

void InteractiveController::set_state(int32 state) { 
  state_ = state; 
}
}
