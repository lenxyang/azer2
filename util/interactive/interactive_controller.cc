#include "azer/util/interactive/interactive_controller.h"

#include "azer/util/interactive/interactive_context.h"
#include "azer/util/interactive/pick_util.h"

namespace azer {
InteractiveController::InteractiveController(InteractiveContext* ctx)
    : context_(ctx),
      activate_frame_(false) {
}

InteractiveController::~InteractiveController() {}

void InteractiveController::OnControllerPicked() {
  FOR_EACH_OBSERVER(InteractiveControllerObserver, observer_list_, 
                    OnControllerPicked(this));
}

void InteractiveController::OnControllerUnpicked() {
  FOR_EACH_OBSERVER(InteractiveControllerObserver, observer_list_, 
                    OnControllerUnpicked(this));
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

void InteractiveController::HitTest(const gfx::Point& pt) {
  state_ = GetPicking(pt);
}

void InteractiveController::Activate() {
  context()->Activate(this);
  activate_frame_ = true;
}

void InteractiveController::Deactivate() {
  context()->Deactivate(this);
}

void InteractiveController::Update(const FrameArgs& args) {
  UpdateFrame(args);
}

void InteractiveController::Render(Renderer* renderer) {
  if (!activate_frame_) {
    RenderFrame(renderer);
  }
  activate_frame_ = false;
}
}
