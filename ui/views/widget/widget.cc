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

namespace views {

Widget::Widget() 
    : closing_(false)
    , observer_manager_(this) {
  default_theme_provider_.reset(new ui::DefaultThemeProvider);
}

Widget::~Widget() {
  root_view_.reset();
  host_->RemoveObserver(this);
  focus_client_.reset();
  event_client_.reset();
}

void Widget::Init(const InitParams& params) {
  host_.reset(aura::WindowTreeHost::Create(params.bounds));
  host_->window()->SetBounds(gfx::Rect(params.bounds.size()));
  focus_client_.reset(new FocusClient);
  aura::client::SetFocusClient(host_->window(), focus_client_.get());

  root_view_.reset(new internal::RootView(this));
  root_view_->Init(params.bounds);
  host_->InitHost();
  host_->window()->AddChild(root_view_->window());
  host_->window()->Show();
  host_->AddObserver(this);

  event_client_.reset(new EventClient(root_view_.get()));
  aura::client::SetEventClient(host_->window(), event_client_.get());
  observer_manager_.Add(GetNativeTheme());
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

ui::ThemeProvider* Widget::GetThemeProvider() const {
  return default_theme_provider_.get();
}

const ui::NativeTheme* Widget::GetNativeTheme() const {
  return ui::NativeThemeAura::instance();
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
}  // namespace views
