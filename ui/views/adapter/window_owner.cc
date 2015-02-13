#include "azer/ui/views/adapter/window_owner.h"

#include <atomic>
#include "base/logging.h"
#include "base/lazy_instance.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/views/view.h"

namespace views {

namespace {
class AuraIdAllocator {
 public:
  AuraIdAllocator() : allocated_id_(0) {}

  int64 allocate_id() { return allocated_id_++;}
 protected:
  std::atomic<int64> allocated_id_;
  DISALLOW_COPY_AND_ASSIGN(AuraIdAllocator);
};

::base::LazyInstance<AuraIdAllocator> id_allocator_ = LAZY_INSTANCE_INITIALIZER;
}  // namespace

WindowOwner::WindowOwner()
    : window_(NULL)
    , view_(NULL)
    , attached_(false){
}

WindowOwner::~WindowOwner() {
  if (window_ && attached_) {
    delete window_;
  }
  bridge_.reset();
}

ui::Layer* WindowOwner::layer() {
  CHECK(window_);
  return window_->layer();
}

const ui::Layer* WindowOwner::layer() const {
  CHECK(!window_);
  return window_->layer();
}

aura::Window* WindowOwner::Create(View* view) {
  CHECK(!window_);
  CHECK(!view_);
  view_ = view;
  bridge_.reset(new ViewBridge(view_));
  window_ = new aura::Window(bridge_.get());
  window_->Init(aura::WINDOW_LAYER_TEXTURED);
  window_->set_id(id_allocator_.Pointer()->allocate_id());
  window_->SetName(view->kViewClassName);
  view->set_id(window_->id());
  attached_ = false;

  return window_;
}

void WindowOwner::OnAddChildViewAt(View* child, int index) {
  window()->AddChild(child->window());
  if (index == view_->child_count()) {
    window()->StackChildAtBottom(child->window());
  } else if (index == 0) {
    window()->StackChildAtTop(child->window());
  } else {
    View* back = view_->child_at(index - 1);
    window()->StackChildBelow(child->window(), back->window());
  }
}

void WindowOwner::OnRemoveChildView(View* child) {
  window()->RemoveChild(child->window());
}

void WindowOwner::Destroy() {
  DCHECK(!attached_);
  delete window_;
  window_ = NULL;
}

void WindowOwner::Attach(aura::Window* window) {
  attached_ = true;
  CHECK(view_);
  CHECK(!window_);
  window_ = window;
}

void WindowOwner::Detach()  {
  DCHECK(attached_);
  window_ = NULL;
}
    
ViewBridge::ViewBridge(View* view)
    : view_(view) {
}

ViewBridge::~ViewBridge() {
}

gfx::Size ViewBridge::GetMinimumSize() const {
  return gfx::Size(1, 1);
}

gfx::Size ViewBridge::GetMaximumSize() const {
  return gfx::Size(800, 600);
}

void ViewBridge::OnBoundsChanged(const gfx::Rect& old_bounds,
                                 const gfx::Rect& new_bounds)  {
}

gfx::NativeCursor ViewBridge::GetCursor(const gfx::Point& point) {
  return gfx::kNullCursor;
}

int ViewBridge::GetNonClientComponent(const gfx::Point& point) const {
  return 0;
}

bool ViewBridge::ShouldDescendIntoChildForEventHandling(aura::Window* child,
                                                    const gfx::Point& location) {
  return true;
}

bool ViewBridge::CanFocus() {
  return false;
}

void ViewBridge::OnCaptureLost() {
}

void ViewBridge::OnPaint(gfx::Canvas* canvas) {
  if (view_) {
    view_->OnPaint(canvas);
  }
}

void ViewBridge::OnDeviceScaleFactorChanged(float device_scale_factor) {
}

void ViewBridge::OnWindowDestroying(aura::Window* window) {
}

void ViewBridge::OnWindowDestroyed(aura::Window* window) {
}

void ViewBridge::OnWindowTargetVisibilityChanged(bool visible) {
}

bool ViewBridge::HasHitTestMask() const {
  return false;
}

void ViewBridge::GetHitTestMask(gfx::Path* mask) const {
}

void ViewBridge::OnKeyEvent(ui::KeyEvent* event) {
  view_->OnKeyEvent(event);
}

void ViewBridge::OnMouseEvent(ui::MouseEvent* event) {
  view_->OnMouseEvent(event);
}

void ViewBridge::OnScrollEvent(ui::ScrollEvent* event) {
  view_->OnScrollEvent(event);
}

void ViewBridge::OnTouchEvent(ui::TouchEvent* event) {
  view_->OnTouchEvent(event);
}

void ViewBridge::OnGestureEvent(ui::GestureEvent* event) {
  view_->OnGestureEvent(event);
}

}  // namespace views
