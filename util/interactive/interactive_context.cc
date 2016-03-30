#include "azer/util/interactive/interactive_context.h"

#include "base/logging.h"
#include "azer/util/interactive/interactive_controller.h"
#include "azer/util/interactive/pick_util.h"

namespace azer {
InteractiveContext::InteractiveContext(nelf::RenderWindow* window,
                                       const Camera* camera)
    : window_(window),
      activated_(-1),
      draging_(false),
      camera_(camera) {
}

void InteractiveContext::Activate(InteractiveController* controller) {
  if (activated_ >= 0) {
    controllers_[activated_]->OnDeactive();
  }

  int index = GetIndexOf(controller);
  DCHECK_GE(index, 0);
  activated_ = index;
  controller->OnActive();
}

void InteractiveContext::Deactivate(InteractiveController* controller) {
  int index = GetIndexOf(controller);
  DCHECK_EQ(index, activated_);
  activated_ = -1;
  controller->OnDeactive();
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

void InteractiveContext::OnKeyPressed(const ui::KeyEvent& event) {
  if (activated_ >= 0) {
    InteractiveController* controller = controllers_[activated_];
    controller->OnKeyPressed(event);
  }
}

void InteractiveContext::OnKeyReleased(const ui::KeyEvent& event) {
  if (activated_ >= 0) {
    InteractiveController* controller = controllers_[activated_];
    controller->OnKeyReleased(event);
  }
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

Ray InteractiveContext::GetPickingRay(const gfx::Point& pt) {
  const gfx::Size size  = window()->GetContentsBounds().size();
  return std::move(azer::GetPickingRay(pt, size, camera_));
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
      controller->OnDragBegin(event);
    }
  }
}

void InteractiveContext::OnDrag(const ui::MouseEvent& event) {
  if (draging_) {
    InteractiveController* controller = controllers_[activated_];
    controller->OnDragging(event);
  }
}

void InteractiveContext::OnDragEnd(const ui::MouseEvent& event) {
  if (draging_) {
    InteractiveController* controller = controllers_[activated_];
    controller->OnDragEnd(event);
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
