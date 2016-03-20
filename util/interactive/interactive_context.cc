#include "azer/util/interactive/interactive_context.h"

#include "base/logging.h"
#include "azer/util/interactive/interactive_controller.h"
#include "azer/util/interactive/pick_util.h"

namespace azer {
InteractiveContext::InteractiveContext()
    : activated_(-1),
      draging_(false) {
}

void InteractiveContext::Activate(InteractiveController* controller) {
  int index = GetIndexOf(controller);
  DCHECK_GE(index, 0);
  activated_ = index;
}

void InteractiveContext::AddController(InteractiveController* controller) {
  controllers_.push_back(controller);
}

void InteractiveContext::RemoveController(InteractiveController* controller) {
  int index = GetIndexOf(controller);
  DCHECK_GE(index, 0);
  controllers_.erase(controllers_.begin() + index);
  if (index < activated_) {
    activated_--;
  } else if (index == activated_) {
    activated_ = -1;
  }
}

int InteractiveContext::GetIndexOf(InteractiveController* controller) const {
  auto i(std::find(controllers_.begin(), controllers_.end(), controller));
  return i != controllers_.end() ? static_cast<int>(i - controllers_.begin()) : -1;
}

void InteractiveContext::OnMousePressed(const ui::MouseEvent& event) {
  OnDragBegin(event);
}

void InteractiveContext::OnMouseDragged(const ui::MouseEvent& event) {
  OnDrag(event);
}

void InteractiveContext::OnMouseReleased(const ui::MouseEvent& event) {
  OnDragEnd(event);
}

void InteractiveContext::OnMouseMoved(const ui::MouseEvent& event) {
  if (activated_ >= 0) {
    InteractiveController* controller = controllers_[activated_];
    controller->HitTest(event.location());
  }
}

void InteractiveContext::OnMouseCaptureLost() {
  // OnDragCancel();
}

void InteractiveContext::OnDragBegin(const ui::MouseEvent& event) {
  if (activated_ >= 0) {
    InteractiveController* controller = controllers_[activated_];
    int ret = controller->GetPicking(event.location());
    if (ret > 0) {
      draging_ = true;
      controller->OnDragBegin(event.location());
    }
  }
}

void InteractiveContext::OnDrag(const ui::MouseEvent& event) {
  if (draging_) {
    InteractiveController* controller = controllers_[activated_];
    controller->OnDrag(event.location());
  }
}

void InteractiveContext::OnDragEnd(const ui::MouseEvent& event) {
  if (draging_) {
    InteractiveController* controller = controllers_[activated_];
    controller->OnDragEnd(event.location());
    draging_ = false;
  }
}

void InteractiveContext::Update(const FrameArgs& args) {
  if (activated_ >= 0) {
    InteractiveController* controller = controllers_[activated_];
    controller->UpdateFrame(args);
  }
}

void InteractiveContext::Render(Renderer* renderer) {
  if (activated_ >= 0) {
    InteractiveController* controller = controllers_[activated_];
    controller->RenderFrame(renderer);
  }
}
}  // namespace azer
