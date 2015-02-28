#include "azer/ui/views/view.h"

#include <atomic>
#include "base/logging.h"
#include "ui/accessibility/ax_enums.h"
#include "ui/native_theme/native_theme.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/path.h"
#include "ui/gfx/scoped_canvas.h"
#include "ui/gfx/screen.h"
#include "ui/gfx/skia_util.h"

#include "azer/ui/aura/window.h"
#include "azer/ui/aura/window_property.h"
#include "azer/ui/views/aura/event_client.h"
#include "azer/ui/views/background.h"
#include "azer/ui/views/border.h"
#include "azer/ui/views/id_allocator.h"
#include "azer/ui/views/painter.h"
#include "azer/ui/views/widget/widget.h"
#include "azer/ui/views/widget/root_view.h"

namespace views {

const char View::kViewClassName[] = "View";

View::View()
    : parent_(NULL)
    , root_(NULL)
    , focusable_(false)
    , visible_(true)
    , enabled_(true)
    , needs_layout_(false)
    , registered_accelerator_count_(0)
    , notify_enter_exit_on_child_(false) 
    , registered_for_visible_bounds_notification_(false)
    , mouse_in_(false) {
  InitAuraWindow(aura::WINDOW_LAYER_TEXTURED);
}

View::~View() {
  if (parent()) {
    parent()->RemoveChildView(this);
  }
  window_.reset();
}

const char* View::GetClassName() const {
  return kViewClassName;
}

Widget* View::GetWidget() {
  return root_->widget_;
}

const Widget* View::GetWidget() const {
  return root_->widget_;
}

void View::InitAuraWindow(aura::WindowLayerType layer_type) {
  window_.reset(new aura::Window(this));
  window()->Init(layer_type);
  window()->set_id(ViewsIDAllocator::Pointer()->allocate_id());
  window()->SetName(GetClassName());
  SetViewInAuraWindowProperty(this);
  window()->Show();
}

int View::id() const {
  return window_->id();
}

void View::set_id(int id) {
  window_->set_id(id);
}

// focus and group
void View::SetGroup(int gid) {
}

int View::GetGroup() const {
  return 0;
}

bool View::IsGroupFocusTraversable() const {
  return false;
}

void View::GetViewsInGroup(int group, Views* views) {
}

View* View::GetSelectedViewForGroup(int group) {
  return NULL;
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
  if (bounds_ != bounds) {
    bounds_ = bounds;
    window()->SetBounds(bounds_);
  }
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

void View::OnBoundsChanged(const gfx::Rect& old_bounds) {
}

void View::PreferredSizeChanged() {
  InvalidateLayout();
  if (parent_)
    parent_->ChildPreferredSizeChanged(this);
}

bool View::GetNeedsNotificationWhenVisibleBoundsChange() const {
  return false;
}

void View::OnVisibleBoundsChanged() {
}

void View::VisibilityChanged(View* starting_from, bool is_visible) {
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
    Layout();
    child->OnAttachedRecusive(root_);
  }

  if (root_) {
    RegisterChildrenForVisibleBoundsNotification(child);
  }
}

void View::RemoveChildView(View* child) {
  DCHECK(child->parent_ == this);
  Views::iterator i =
    std::find(children_.begin(), children_.end(), child);
  DCHECK(i != children_.end());
  children_.erase(i);

  if (root_) {
    UnregisterChildrenForVisibleBoundsNotification(child);
  }

  window()->RemoveChild(child->window());
  child->OnDetachRecusive();
}

void View::RemoveAllChildViews() {
  while (!children_.empty()) {
    RemoveChildView(children_[0]);
  }
}

bool View::Contains(View* view) const{
  return window()->Contains(view->window());
}

void View::OnAttachedRecusive(internal::RootView* view) {
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
  // from aura::Window::Delegate
  BoundsChanged(old_bounds);
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
      if (!mouse_in_) {
        mouse_in_ = true;
        OnMouseEntered(*event);
        PropagateMouseEntered(*event);
      }
      break;

    case ui::ET_MOUSE_EXITED:
      mouse_in_ = false;
      OnMouseExited(*event);
      PropagateMouseExited(*event);
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
  if (background_.get()) {
    background_->Paint(canvas, this);
  }
}

void View::OnPaintBorder(gfx::Canvas* canvas) {
  if (border_.get()) {
    border_->Paint(*this, canvas);
  }
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
  if (visible()) {
    return gfx::Rect();
  }

  gfx::Rect vis_bounds(GetLocalBounds());
  gfx::Rect ancestor_bounds;
  const View* view = this;

  while (view != NULL && view != root_ && !vis_bounds.IsEmpty()) {
    const View* ancestor = view->parent_;
    aura::Window::ConvertRectToTarget(view->window(),
                                      ancestor->window(),
                                      &vis_bounds);
    if (ancestor != NULL) {
      ancestor_bounds.SetRect(0, 0, ancestor->width(), ancestor->height());
      vis_bounds.Intersect(ancestor_bounds);
    } else if (!view->GetWidget()) {
      // If the view has no Widget, we're not visible. Return an empty rect.
      return gfx::Rect();
    }
    view = ancestor;
  }

  if (vis_bounds.IsEmpty())
    return vis_bounds;

  return vis_bounds;
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

bool View::HitTestPoint(const gfx::Point& local_point) const {
  gfx::Rect local_bounds(bounds().size());
  if (!HasHitTestMask())
    return local_bounds.Contains(local_point);

  gfx::Path mask;
  GetHitTestMask(&mask);

  SkRegion clip_region;
  clip_region.setRect(local_bounds.x(), local_bounds.y(),
                      local_bounds.width(), local_bounds.height());
  SkRegion mask_region;
  return mask_region.setPath(mask, clip_region) &&
      mask_region.contains(local_point.x(), local_point.y());
}

bool View::HitTestRect(const gfx::Rect& local_rect) const {
  if (!HasHitTestMask()) {
    return local_rect.Intersects(bounds_);
  } else {
    // Early return if |mask| is not a valid hit test mask.
    gfx::Path mask;
    GetHitTestMask(&mask);

    // Return whether or not |rect| intersects the custom hit test mask
    // of |target|.
    SkRegion clip_region;
    clip_region.setRect(0, 0, bounds_.width(), bounds_.height());
    SkRegion mask_region;
    return mask_region.setPath(mask, clip_region) &&
        mask_region.intersects(RectToSkIRect(local_rect));
  }
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

// RTL positioning -------------------------------------------------------------

gfx::Rect View::GetMirroredBounds() const {
  gfx::Rect bounds(bounds_);
  bounds.set_x(GetMirroredX());
  return bounds;
}

gfx::Point View::GetMirroredPosition() const {
  return gfx::Point(GetMirroredX(), y());
}

int View::GetMirroredX() const {
  return parent_ ? parent_->GetMirroredXForRect(bounds_) : x();
}

int View::GetMirroredXForRect(const gfx::Rect& bounds) const {
  return base::i18n::IsRTL() ?
      (width() - bounds.x() - bounds.width()) : bounds.x();
}

int View::GetMirroredXInView(int x) const {
  return base::i18n::IsRTL() ? width() - x : x;
}

int View::GetMirroredXWithWidthInView(int x, int w) const {
  return base::i18n::IsRTL() ? width() - x - w : x;
}

void View::Layout() {
  needs_layout_ = false;

  // If we have a layout manager, let it handle the layout for us.

  // Make sure to propagate the Layout() call to any children that haven't
  // received it yet through the layout manager and need to be laid out. This
  // is needed for the case when the child requires a layout but its bounds
  // weren't changed by the layout manager. If there is no layout manager, we
  // just propagate the Layout() call down the hierarchy, so whoever receives
  // the call can take appropriate action.
  for (int i = 0, count = child_count(); i < count; ++i) {
    View* child = child_at(i);
    child->Layout();
    /*
    if (child->needs_layout_ || !layout_manager_.get()) {
      child->needs_layout_ = false;
      child->Layout();
    }
    */
  }
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

// Size and disposition --------------------------------------------------------

void View::PropagateVisibilityNotifications(View* start, bool is_visible) {
  for (int i = 0, count = child_count(); i < count; ++i)
    child_at(i)->PropagateVisibilityNotifications(start, is_visible);
  VisibilityChangedImpl(start, is_visible);
}

void View::VisibilityChangedImpl(View* starting_from, bool is_visible) {
  VisibilityChanged(starting_from, is_visible);
}

void View::BoundsChanged(const gfx::Rect& previous_bounds) {
  OnBoundsChanged(previous_bounds);

  if (previous_bounds.size() != size()) {
    needs_layout_ = false;
    Layout();
  }

  if (GetNeedsNotificationWhenVisibleBoundsChange())
    OnVisibleBoundsChanged();

  // Notify interested Views that visible bounds within the root view may have
  // changed.
  if (descendants_to_notify_.get()) {
    for (Views::iterator i(descendants_to_notify_->begin());
         i != descendants_to_notify_->end(); ++i) {
      (*i)->OnVisibleBoundsChanged();
    }
  }
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

void View::InvalidateLayout() {
  // Always invalidate up. This is needed to handle the case of us already being
  // valid, but not our parent.
  needs_layout_ = true;
  if (parent_)
    parent_->InvalidateLayout();
}

bool View::CanProcessEventsWithinSubtree() const {
  return true;
}

void View::PropagateMouseEntered(const ui::MouseEvent& event) {
  View* top_view = NULL;
  for (View* p = parent(); p && p->window(); p = p->parent()) {
    if (p->notify_enter_exit_on_child()) {
      top_view = p;
    }
  }

  if (top_view) {
    for (View* p = parent(); p; p = p->parent()) {
      p->OnMouseEntered(event);
	  p->mouse_in_ = true;
      if (p == top_view) {
        break;
      }
    }
  }
}

void View::PropagateMouseExited(const ui::MouseEvent& event) {
  View* top_view = NULL;
  for (View* p = parent(); p && p->window(); p = p->parent()) {
    if (p->notify_enter_exit_on_child()
        && !p->window()->ContainsPointInRoot(event.root_location())) {
      top_view = p;
    }
  }

  if (top_view) {
    for (View* p = parent(); p; p = p->parent()) {
      if (!p->window()->ContainsPointInRoot(event.root_location())) {
        p->OnMouseExited(event);
		p->mouse_in_ = false;
        if (p == top_view) {
          break;
        }
      }
    }
  }
}

// Tooltips --------------------------------------------------------------------

bool View::GetTooltipText(const gfx::Point& p, base::string16* tooltip) const {
  return false;
}

bool View::GetTooltipTextOrigin(const gfx::Point& p, gfx::Point* loc) const {
  return false;
}

// System events ---------------------------------------------------------------

void View::PropagateThemeChanged() {
  for (int i = child_count() - 1; i >= 0; --i)
    child_at(i)->PropagateThemeChanged();
  OnThemeChanged();
}

// Accelerators ----------------------------------------------------------------

void View::AddAccelerator(const ui::Accelerator& accelerator) {
  if (!accelerators_.get())
    accelerators_.reset(new std::vector<ui::Accelerator>());

  if (std::find(accelerators_->begin(), accelerators_->end(), accelerator) ==
      accelerators_->end()) {
    accelerators_->push_back(accelerator);
  }
  RegisterPendingAccelerators();
}

void View::RemoveAccelerator(const ui::Accelerator& accelerator) {
}

void View::ResetAccelerators() {
  if (accelerators_.get())
    UnregisterAccelerators(false);
}

bool View::AcceleratorPressed(const ui::Accelerator& accelerator) {
  return false;
}

bool View::CanHandleAccelerators() const {
  return enabled() && visible() && GetWidget() && GetWidget()->IsVisible();
}

void View::RegisterPendingAccelerators() {
}

void View::UnregisterAccelerators(bool leave_data_intact) {
}

// static
void View::RegisterChildrenForVisibleBoundsNotification(View* view) {
  if (view->GetNeedsNotificationWhenVisibleBoundsChange())
    view->RegisterForVisibleBoundsNotification();
  for (int i = 0; i < view->child_count(); ++i)
    RegisterChildrenForVisibleBoundsNotification(view->child_at(i));
}

// static
void View::UnregisterChildrenForVisibleBoundsNotification(View* view) {
  if (view->GetNeedsNotificationWhenVisibleBoundsChange())
    view->UnregisterForVisibleBoundsNotification();
  for (int i = 0; i < view->child_count(); ++i)
    UnregisterChildrenForVisibleBoundsNotification(view->child_at(i));
}

void View::RegisterForVisibleBoundsNotification() {
  if (registered_for_visible_bounds_notification_)
    return;

  registered_for_visible_bounds_notification_ = true;
  for (View* ancestor = parent_; ancestor; ancestor = ancestor->parent_)
    ancestor->AddDescendantToNotify(this);
}

void View::UnregisterForVisibleBoundsNotification() {
  if (!registered_for_visible_bounds_notification_)
    return;

  registered_for_visible_bounds_notification_ = false;
  for (View* ancestor = parent_; ancestor; ancestor = ancestor->parent_)
    ancestor->RemoveDescendantToNotify(this);
}

void View::AddDescendantToNotify(View* view) {
  DCHECK(view);
  if (!descendants_to_notify_.get())
    descendants_to_notify_.reset(new Views);
  descendants_to_notify_->push_back(view);
}

void View::RemoveDescendantToNotify(View* view) {
  DCHECK(view && descendants_to_notify_.get());
  Views::iterator i(std::find(
      descendants_to_notify_->begin(), descendants_to_notify_->end(), view));
  DCHECK(i != descendants_to_notify_->end());
  descendants_to_notify_->erase(i);
  if (descendants_to_notify_->empty())
    descendants_to_notify_.reset();
}
}  // namespace views
