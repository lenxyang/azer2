#include "azer/ui/views/view.h"

#include <atomic>
#include "base/logging.h"
#include "ui/accessibility/ax_enums.h"
#include "ui/native_theme/native_theme.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/aura/window_property.h"

#include "azer/ui/views/background.h"
#include "azer/ui/views/border.h"
#include "azer/ui/views/id_allocator.h"
#include "azer/ui/views/painter.h"
#include "azer/ui/views/root_view.h"

DECLARE_WINDOW_PROPERTY_TYPE(view::View*)

namespace views {

DEFINE_WINDOW_PROPERTY_KEY(Views*, kAzerView, NULL);

View::View()
    : parent_(NULL)
    , root_(NULL)
    , focusable_(false)
    , visible_(false)
    , enabled_(true) {
  InitAuraWindow();
}

View::~View() {
}

const char* View::GetClassName() const {
  return "View";
}

void View::InitAuraWindow() {
  window_.reset(new aura::Window(this));
  window()->Init(aura::WINDOW_LAYER_TEXTURED);
  window()->set_id(ViewsIDAllocator::Pointer()->allocate_id());
  window()->SetName(GetClassName());

  window()->SetProperty(kAzerView, this);
}

int View::id() const {
  return window_->id();
}

void View::set_id(int id) {
  window_->set_id(id);
}

const std::string& View::name() const {
  return window_->name();
}

void View::SetName(const std::string& name) {
  window_->SetName(name);
}

void View::SetBounds(int x, int y, int width, int height) {
  SetBoundsRect(gfx::Rect(x, y, std::max(0, width), std::max(0, height)));
}

void View::SetBoundsRect(const gfx::Rect& bounds) {
  bounds_ = bounds;
  window()->SetBounds(bounds);
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

void View::SizeToPreferredSize() {
  gfx::Size prefsize = GetPreferredSize();
  if ((prefsize.width() != width()) || (prefsize.height() != height()))
    SetBounds(x(), y(), prefsize.width(), prefsize.height());
}

void View::OnBoundsChanged(const gfx::Rect& ) {
}

void View::Show() {
  window()->Show();
  visible_ = true;
}

void View::Hide() {
  window()->Hide();
  visible_ = false;
}

void View::AddChildView(View* child) {
  DCHECK(child->parent_ == NULL);
  child->parent_ = this;
  children_.push_back(child);

  window()->AddChild(child->window());
  if (root_) {
    child->OnAttachedRecusive(root_);
  }
}

void View::RemoveChildView(View* child) {
  DCHECK(child->parent_ == this);
  Views::iterator i =
    std::find(children_.begin(), children_.end(), child);
  DCHECK(i != children_.end());
  children_.erase(i);

  window()->RemoveChild(child->window());
  child->OnDetachRecusive();
}

bool View::Contains(View* view) {
  return window()->Contains(view->window());
}

void View::OnAttachedRecusive(RootView* view) {
  root_ = view;
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    (*iter)->OnAttachedRecusive(view);
  }
}

void View::OnDetachRecusive() {
  root_ = NULL;
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    (*iter)->OnDetachRecusive();
  }
}

gfx::Size View::GetMinimumSize() const {
  return gfx::Size(1, 1);
}

gfx::Size View::GetMaximumSize() const {
  return gfx::Size(800, 600);
}

void View::OnBoundsChanged(const gfx::Rect& old_bounds,
                             const gfx::Rect& new_bounds)  {
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
  return window()->CanFocus();
}

void View::RequestFocus() {
}

void View::SetEnabled(bool enabled) {
  if (enabled_ != enabled) {
    enabled_ = enabled;
    OnEnabledChanged();
  }
}

void View::OnEnabledChanged() {
  SchedulePaint();
}

void View::OnCaptureLost() {
}

// void View::OnPaint(gfx::Canvas* canvas) {
// }

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

bool View::IsMouseHovered() {
  return false;
}

void View::OnKeyEvent(ui::KeyEvent* event) {
  bool consumed = (event->type() == ui::ET_KEY_PRESSED) ? OnKeyPressed(*event) :
      OnKeyReleased(*event);
  if (consumed)
    event->StopPropagation();
}

void View::OnMouseEvent(ui::MouseEvent* event) {
  switch (event->type()) {
    case ui::ET_MOUSE_PRESSED:
      if (ProcessMousePressed(*event))
        event->SetHandled();
      return;

    case ui::ET_MOUSE_MOVED:
      if ((event->flags() & (ui::EF_LEFT_MOUSE_BUTTON |
                             ui::EF_RIGHT_MOUSE_BUTTON |
                             ui::EF_MIDDLE_MOUSE_BUTTON)) == 0) {
        OnMouseMoved(*event);
        return;
      }
      // FALL-THROUGH
    case ui::ET_MOUSE_DRAGGED:
      if (ProcessMouseDragged(*event))
        event->SetHandled();
      return;

    case ui::ET_MOUSE_RELEASED:
      ProcessMouseReleased(*event);
      return;

    case ui::ET_MOUSEWHEEL:
      if (OnMouseWheel(*static_cast<ui::MouseWheelEvent*>(event)))
        event->SetHandled();
      break;

    case ui::ET_MOUSE_ENTERED:
      OnMouseEntered(*event);
      break;

    case ui::ET_MOUSE_EXITED:
      OnMouseExited(*event);
      break;

    default:
      return;
  }
}

void View::OnScrollEvent(ui::ScrollEvent* event) {
}

void View::OnTouchEvent(ui::TouchEvent* event) {
}

void View::OnGestureEvent(ui::GestureEvent* event) {
}


gfx::Size View::GetPreferredSize() const {
  return gfx::Size(0, 0);
}

void View::set_background(Background* b) {
  background_.reset(b);
}

void View::SetBorder(scoped_ptr<Border> b) { border_ = b.Pass(); }

void View::OnPaint(gfx::Canvas* canvas) {
  OnPaintBackground(canvas);
  OnPaintBorder(canvas);
}

void View::OnPaintBackground(gfx::Canvas* canvas) {
}

void View::OnPaintBorder(gfx::Canvas* canvas) {
}

bool View::HasFocus() const {
  DCHECK(window());
  return window()->HasFocus();
}

gfx::Rect View::GetContentsBounds() const {
  gfx::Rect contents_bounds(GetLocalBounds());
  if (border_.get())
    contents_bounds.Inset(border_->GetInsets());
  return contents_bounds;
}

gfx::Rect View::GetLocalBounds() const {
  return gfx::Rect(size());
}

gfx::Rect View::GetVisibleBounds() const {
  return bounds_;
}

gfx::Rect View::GetBoundsInScreen() const {
  DCHECK(window());
  return window()->GetBoundsInScreen();
}

gfx::Insets View::GetInsets() const {
  return border_.get() ? border_->GetInsets() : gfx::Insets();
}

gfx::NativeCursor View::GetCursor(const ui::MouseEvent& event) {
#if defined(OS_WIN)
  static ui::Cursor arrow;
  if (!arrow.platform())
    arrow.SetPlatformCursor(LoadCursor(NULL, IDC_ARROW));
  return arrow;
#else
  return gfx::kNullCursor;
#endif
}

bool View::HitTestPoint(const gfx::Point& point) const {
  return HitTestRect(gfx::Rect(point, gfx::Size(1, 1)));
}

bool View::HitTestRect(const gfx::Rect& rect) const {
  return true;
}

bool View::OnMousePressed(const ui::MouseEvent& event) {
  return false;
}

bool View::OnMouseDragged(const ui::MouseEvent& event) {
  return false;
}

void View::OnMouseReleased(const ui::MouseEvent& event) {
}

void View::OnMouseCaptureLost() {
}

void View::OnMouseMoved(const ui::MouseEvent& event) {
}

void View::OnMouseEntered(const ui::MouseEvent& event) {
}

void View::OnMouseExited(const ui::MouseEvent& event) {
}

bool View::OnKeyPressed(const ui::KeyEvent& event) {
  return false;
}

bool View::OnKeyReleased(const ui::KeyEvent& event) {
  return false;
}

bool View::OnMouseWheel(const ui::MouseWheelEvent& event) {
  return false;
}

bool View::ProcessMousePressed(const ui::MouseEvent& event)  {
  return OnMousePressed(event);
}

bool View::ProcessMouseDragged(const ui::MouseEvent& event) {
  if (OnMouseDragged(event))
    return true;

  return false;
}

void View::ProcessMouseReleased(const ui::MouseEvent& event) {
  OnMouseReleased(event);
}

void View::Layout() {
}

void View::SchedulePaint() {
  DCHECK(window());
  window()->SchedulePaintInRect(window()->bounds());
}

int View::GetBaseline() const {
  return -1;
}

int View::GetHeightForWidth(int w) const {
  return GetPreferredSize().height();
}

ui::ThemeProvider* View::GetThemeProvider() const {
  return root_ ? root_->GetThemeProvider() : NULL;
}

const ui::NativeTheme* View::GetNativeTheme() const {
  return root_ ? root_->GetNativeTheme() : ui::NativeTheme::instance();
}

void View::PropagateNativeThemeChanged(const ui::NativeTheme* theme) {
  for (int i = 0, count = child_count(); i < count; ++i)
    child_at(i)->PropagateNativeThemeChanged(theme);
  OnNativeThemeChanged(theme);
}

void View::PreferredSizeChanged() {
  if (parent_)
    parent_->ChildPreferredSizeChanged(this);
}

bool View::CanDrop(const ui::OSExchangeData& data) {
  // TODO(sky): when I finish up migration, this should default to true.
  return false;
}

void View::OnDragEntered(const ui::DropTargetEvent& event) {
}

int View::OnDragUpdated(const ui::DropTargetEvent& event) {
  return ui::DragDropTypes::DRAG_NONE;
}

void View::OnDragExited() {
}

int View::OnPerformDrop(const ui::DropTargetEvent& event) {
  return ui::DragDropTypes::DRAG_NONE;
}

void View::OnDragDone() {
}

bool View::InDrag() {
  return false;
}

void View::OnBlur() {
}

void View::OnFocus() {
}

void View::VisibilityChanged(View* starting_from, bool is_visible) {
}

void View::InvalidateLayout() {
}

bool View::CanProcessEventsWithinSubtree() const {
  return true;
}
}  // namespace views
