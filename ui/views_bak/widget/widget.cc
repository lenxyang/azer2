#include "azer/ui/views/widget/widget.h"


#include "ui/native_theme/native_theme_aura.h"
#include "ui/base/default_theme_provider.h"
#include "ui/base/hit_test.h"
#include "ui/base/l10n/l10n_font_util.h"
#include "ui/base/resource/resource_bundle.h"

#include "azer/ui/aura/window_tree_host.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/views/id_allocator.h"
#include "azer/ui/views/widget/root_view.h"
#include "azer/ui/views/aura/focus_client.h"
#include "azer/ui/views/aura/event_client.h"
#include "azer/ui/views/focus/focus_manager.h"
#include "azer/ui/views/ime/input_method.h"
#include "azer/ui/views/widget/widget_delegate.h"

namespace views {

// A default implementation of WidgetDelegate, used by Widget when no
// WidgetDelegate is supplied.
class DefaultWidgetDelegate : public WidgetDelegate {
 public:
  explicit DefaultWidgetDelegate(Widget* widget) : widget_(widget) {
  }
  ~DefaultWidgetDelegate() override {}

  // Overridden from WidgetDelegate:
  void DeleteDelegate() override { delete this; }
  Widget* GetWidget() override { return widget_; }
  const Widget* GetWidget() const override { return widget_; }
  bool ShouldAdvanceFocusToTopLevelWidget() const override {
    // In most situations where a Widget is used without a delegate the Widget
    // is used as a container, so that we want focus to advance to the top-level
    // widget. A good example of this is the find bar.
    return true;
  }

 private:
  Widget* widget_;

  DISALLOW_COPY_AND_ASSIGN(DefaultWidgetDelegate);
};

Widget::InitParams::InitParams()
    : delegate(NULL) {
}

Widget::Widget() 
    : closing_(false)
    , observer_manager_(this)
    , widget_delegate_(NULL)
    , is_top_level_(true) {
  default_theme_provider_.reset(new ui::DefaultThemeProvider);
}

Widget::~Widget() {
  root_view_.reset();
  host_->RemoveObserver(this);
  focus_client_.reset();
  event_client_.reset();
}

void Widget::Init(const InitParams& params) {
  widget_delegate_ = params.delegate ?
      params.delegate : new DefaultWidgetDelegate(this);

  host_.reset(aura::WindowTreeHost::Create(params.bounds));
  focus_client_.reset(new FocusClient);
  aura::client::SetFocusClient(host_->window(), focus_client_.get());

  root_view_.reset(CreateRootView());
  root_view_->Init(params.bounds);
  host_->InitHost();
  host_->window()->AddChild(root_view_->window());
  host_->window()->set_id(ViewsIDAllocator::Pointer()->allocate_id());
  host_->window()->SetBounds(gfx::Rect(params.bounds.size()));
  host_->window()->Show();
  host_->AddObserver(this);

  event_client_.reset(new EventClient(root_view_.get()));
  aura::client::SetEventClient(host_->window(), event_client_.get());
  observer_manager_.Add(GetNativeTheme());
}

void Widget::SetBounds(const gfx::Rect& bounds) {
  root_view_->SetBoundsRect(bounds);
}

void Widget::SetSize(const gfx::Size& size) {
  root_view_->SetBoundsRect(gfx::Rect(host_->window()->bounds().origin(), size));
}

void Widget::OnRootViewSetBoundsChanged(const gfx::Rect& bounds) {
  host_->SetBounds(bounds);
  host_->window()->SetBounds(gfx::Rect(bounds.size()));
}

void Widget::CenterWindow(const gfx::Size& size) {
}

void Widget::Close() {
  closing_ = true;
}

void Widget::CloseNow() {
  Close();
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

void Widget::SetAlwaysOnTop(bool on_top) {
}

bool Widget::IsAlwaysOnTop() const {
  return false;
}

void Widget::SetVisibleOnAllWorkspaces(bool always_visible) {
}

void Widget::Maximize() {
}

void Widget::Minimize() {
}

void Widget::Restore() {
}

bool Widget::IsMaximized() const {
  return false;
}

bool Widget::IsMinimized() const {
  return false;
}

void Widget::SetFullscreen(bool fullscreen) {
}

bool Widget::IsFullscreen() const {
  return false;
}


ui::ThemeProvider* Widget::GetThemeProvider() const {
  return default_theme_provider_.get();
}

const ui::NativeTheme* Widget::GetNativeTheme() const {
  return ui::NativeThemeAura::instance();
}

FocusManager* Widget::GetFocusManager() {
  Widget* toplevel_widget = GetTopLevelWidget();
  return toplevel_widget ? toplevel_widget->focus_manager_.get() : NULL;
}

const FocusManager* Widget::GetFocusManager() const {
  const Widget* toplevel_widget = GetTopLevelWidget();
  return toplevel_widget ? toplevel_widget->focus_manager_.get() : NULL;
}

InputMethod* Widget::GetInputMethod() {
  return const_cast<InputMethod*>(
      const_cast<const Widget*>(this)->GetInputMethod());
}

const InputMethod* Widget::GetInputMethod() const {
  if (is_top_level()) {
    if (!input_method_.get())
      input_method_ = const_cast<Widget*>(this)->CreateInputMethod().Pass();
    return input_method_.get();
  } else {
    const Widget* toplevel = GetTopLevelWidget();
    // If GetTopLevelWidget() returns itself which is not toplevel,
    // the widget is detached from toplevel widget.
    // TODO(oshima): Fix GetTopLevelWidget() to return NULL
    // if there is no toplevel. We probably need to add GetTopMostWidget()
    // to replace some use cases.
    return (toplevel && toplevel != this) ? toplevel->GetInputMethod() : NULL;
  }
}

ui::InputMethod* Widget::GetHostInputMethod() {
  return NULL;
}

void Widget::OnNativeThemeUpdated(ui::NativeTheme* observed_theme) {
  DCHECK(observer_manager_.IsObserving(observed_theme));

  ui::NativeTheme* current_native_theme = GetNativeTheme();
  if (!observer_manager_.IsObserving(current_native_theme)) {
    observer_manager_.RemoveAll();
    observer_manager_.Add(current_native_theme);
  }

  root_view_->PropagateNativeThemeChanged(current_native_theme);
}

bool Widget::IsVisible() const {
  DCHECK(host_.get());
  return host_->window()->IsVisible();
}

Widget* Widget::GetTopLevelWidget() {
  return const_cast<Widget*>(
      static_cast<const Widget*>(this)->GetTopLevelWidget());
}

const Widget* Widget::GetTopLevelWidget() const {
  // GetTopLevelNativeWidget doesn't work during destruction because
  // property is gone after gobject gets deleted. Short circuit here
  // for toplevel so that InputMethod can remove itself from
  // focus manager.
  return this;
}

void Widget::SetContentsView(View* view) {
  if (view == GetContentsView())
    return;
  root_view_->SetContentsView(view);
}

View* Widget::GetContentsView() {
  return root_view_->GetContentsView();
}

gfx::Rect Widget::GetWindowBoundsInScreen() const {
  return host_->window()->GetBoundsInScreen();
}

gfx::Rect Widget::GetClientAreaBoundsInScreen() const {
  return root_view_->window()->GetBoundsInScreen();
}

gfx::Rect Widget::GetWorkAreaBoundsInScreen() const {
  return GetClientAreaBoundsInScreen();
}

internal::RootView* Widget::CreateRootView() {
  return new internal::RootView(this);
}

scoped_ptr<InputMethod> Widget::CreateInputMethod() {
  /*
  aura::Window* root_window = window_->GetRootWindow();
  ui::InputMethod* host =
      root_window->GetProperty(aura::client::kRootWindowInputMethodKey);
  scoped_ptr<InputMethod> input_method(new InputMethodBridge(this, host, true));
  input_method->Init(this);

  return input_method.Pass();
  */
  return scoped_ptr<InputMethod>().Pass();
}

void Widget::ReplaceInputMethod(InputMethod* input_method) {
  /*
  input_method_.reset(input_method);
  input_method->SetDelegate(native_widget_->GetInputMethodDelegate());
  input_method->Init(this);
  */
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetAura, views::InputMethodDelegate implementation:

void Widget::DispatchKeyEventPostIME(const ui::KeyEvent& key) {
  /*
  FocusManager* focus_manager = GetWidget()->GetFocusManager();
  delegate_->OnKeyEvent(const_cast<ui::KeyEvent*>(&key));
  if (key.handled() || !focus_manager)
    return;
  focus_manager->OnKeyEvent(key);
  */
}

FocusTraversable* Widget::GetFocusTraversable() {
  return static_cast<internal::RootView*>(root_view_.get());
}

void Widget::SetFocusTraversableParent(FocusTraversable* parent) {
  root_view_->SetFocusTraversableParent(parent);
}

void Widget::SetFocusTraversableParentView(View* parent_view) {
  root_view_->SetFocusTraversableParentView(parent_view);
}

void Widget::ClearNativeFocus() {
}
}  // namespace views
