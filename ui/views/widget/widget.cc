#include "azer/ui/views/widget/widget.h"

// #include "ui/wm/core/shadow_types.h"
#include "azer/ui/aura/window_tree_host.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/views/widget/aura/focus_client.h"

namespace views {

Widget::Widget() 
    : cursor_(gfx::kNullCursor) 
    , widget_closed_(false) 
    , closed_requested_(false) {
}

Widget::~Widget() {
  root_view_->WindowOwner::Detach();
  content_window_container_.reset();
  host_->RemoveObserver(this);
  focus_client_.reset();
  host_.reset();
}

void Widget::Init(const InitParams& params) {
  host_.reset(aura::WindowTreeHost::Create(params.bounds));
  focus_client_.reset(new FocusClient);
  aura::client::SetFocusClient(host_->window(), focus_client_.get());
  
  root_view_.reset(new RootView(this));
  content_window_ = root_view_->window();
  content_window_->SetBounds(gfx::Rect(params.bounds.size()));
  content_window_->SetName("ContentWindow");
  // wm::SetShadowType(content_window_.get(), wm::SHADOW_TYPE_NONE);

  content_window_container_.reset(new aura::Window(NULL));
  content_window_container_->Init(aura::WINDOW_LAYER_NOT_DRAWN);
  content_window_container_->Show();
  content_window_container_->AddChild(content_window_);
  content_window_container_->SetBounds(gfx::Rect(params.bounds.size()));
  content_window_container_->SetName("ContentWindowContainer");

  host_->InitHost();
  host_->window()->AddChild(content_window_container_.get());
  
  host_->AddObserver(this);
}

void Widget::Close() {
  if (widget_closed_) {
    return;
  }

  // host_->Close();
  widget_closed_ = true;
}

bool Widget::IsClosed() const {
  return widget_closed_;
}

bool Widget::IsClosing() const {
  return closed_requested_;
}

void Widget::Show() {
  host_->Show();
}

void Widget::Hide() {
  host_->Hide();
}

void Widget::ShowInactive() {
}

void Widget::Activate() {
}

void Widget::Deactivate() {
}

bool Widget::IsActive() const {
  return true;
}

bool Widget::CanActivate() const {
  return true;
}

bool Widget::IsVisible() {
  return host_->window()->IsVisible();
}

void Widget::UpdateRootLayers() {
}

void Widget::NotifyWillRemoveView(View* view) {
}

RootView* Widget::GetRootView() { 
  return root_view_.get();
}

const RootView* Widget::GetRootView() const { 
  return root_view_.get();
}

bool Widget::IsMouseEventsEnabled() { 
  return true;
}

InputMethod* Widget::GetInputMethod() { 
  return NULL;
}

const InputMethod* Widget::GetInputMethod() const { 
  return NULL;
}


// Overridden from aura::WindowDelegate:
gfx::Size Widget::GetMinimumSize() const {
  return gfx::Size(1, 1);
}

gfx::Size Widget::GetMaximumSize() const {
  return gfx::Size(800, 600);
}

void Widget::OnBoundsChanged(const gfx::Rect& old_bounds,
                             const gfx::Rect& new_bounds)  {
}

gfx::NativeCursor Widget::GetCursor(const gfx::Point& point) {
  return cursor_;
}

int Widget::GetNonClientComponent(const gfx::Point& point) const {
  return 0;
}

bool Widget::ShouldDescendIntoChildForEventHandling(aura::Window* child,
                                                    const gfx::Point& location) {
  return true;
}

bool Widget::CanFocus() {
  return false;
}

void Widget::OnCaptureLost() {
}

void Widget::OnPaint(gfx::Canvas* canvas) {
}

void Widget::OnDeviceScaleFactorChanged(float device_scale_factor) {
}

void Widget::OnWindowDestroying(aura::Window* window) {
}

void Widget::OnWindowDestroyed(aura::Window* window) {
}

void Widget::OnWindowTargetVisibilityChanged(bool visible) {
}

bool Widget::HasHitTestMask() const {
  return false;
}

void Widget::GetHitTestMask(gfx::Path* mask) const {
}


  // Overridden from aura::WindowTreeHostObserver:
void Widget::OnHostCloseRequested(const aura::WindowTreeHost* host) {
  closed_requested_ = true;
}

void Widget::OnHostResized(const aura::WindowTreeHost* host) {
}

void Widget::OnHostMoved(const aura::WindowTreeHost* host,
                         const gfx::Point& new_origin) {
}

  // Overridden from ui::EventHandler:
void Widget::OnKeyEvent(ui::KeyEvent* event) {
}

void Widget::OnMouseEvent(ui::MouseEvent* event) {
}

void Widget::OnScrollEvent(ui::ScrollEvent* event) {
}

void Widget::OnGestureEvent(ui::GestureEvent* event) {
}
}  // namespace views {
