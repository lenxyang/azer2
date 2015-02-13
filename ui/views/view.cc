#include "azer/ui/views/view.h"

#include "base/logging.h"
#include "base/lazy_instance.h"
#include "azer/ui/aura/window.h"

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

View::View() {
  window_.reset(new aura::Window(this));
  window_->Init(aura::WINDOW_LAYER_TEXTURED);
  window_->set_id(id_allocator_.Pointer()->allocate_id());
  window_->SetName(view->kViewClassName);
  window_->Show();
}

View::~View() {
}

void View::SetBounds(int x, int y, int width, int height) {
  SetBoundsRect(gfx::Rect(x, y, std::max(0, width), std::max(0, height)));
}

void View::SetBoundsRect(const gfx::Rect& bounds) {
  bounds_ = bounds;
}

void View::SetSize(const gfx::Size& size) {
  SetBounds(x(), y(), size.width(), size.height());
}

void View::SetPosition(const gfx::Point& position) {
  SetBounds(position.x(), position.y(), width(), height());
}

void View::SetX(int x) {
  SetBounds(x, y(), width(), height());
}

void View::SetY(int y) {
  SetBounds(x(), y, width(), height());
}

gfx::Size View::GetMinimumSize() const {
  return gfx::Size(1, 1);
}

gfx::Size View::GetMaximumSize() const {
  return gfx::Size(800, 600);
}

gfx::NativeCursor View::GetCursor(const gfx::Point& point) {
  return gfx::kNullCursor;
}

int View::GetNonClientComponent(const gfx::Point& point) const {
  return 0;
}

bool View::ShouldDescendIntoChildForEventHandling(aura::Window* child,
                                                  const gfx::Point& location) {
  return true;
}

bool View::CanFocus() {
  return false;
}

void View::OnCaptureLost() {
}

void View::OnPaint(gfx::Canvas* canvas) {
  if (view_) {
    view_->OnPaint(canvas);
  }
}

void View::OnDeviceScaleFactorChanged(float device_scale_factor) {
}

void View::OnWindowDestroying(aura::Window* window) {
}

void View::OnWindowDestroyed(aura::Window* window) {
}

void View::OnWindowTargetVisibilityChanged(bool visible) {
}

bool View::HasHitTestMask() const {
  return false;
}

void View::GetHitTestMask(gfx::Path* mask) const {
}

void View::OnKeyEvent(ui::KeyEvent* event) {
}

void View::OnMouseEvent(ui::MouseEvent* event) {
}

void View::OnScrollEvent(ui::ScrollEvent* event) {
}

void View::OnTouchEvent(ui::TouchEvent* event) {
}

void View::OnGestureEvent(ui::GestureEvent* event) {
}
}  // namespace views
