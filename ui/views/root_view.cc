#include "azer/ui/views/root_view.h"


#include "ui/native_theme/native_theme_aura.h"
#include "ui/base/default_theme_provider.h"
#include "ui/base/hit_test.h"
#include "ui/base/l10n/l10n_font_util.h"
#include "ui/base/resource/resource_bundle.h"

#include "azer/ui/aura/window_tree_host.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/views/aura/focus_client.h"
#include "azer/ui/views/aura/event_client.h"
#include "azer/ui/views/id_allocator.h"

namespace views {

RootView::RootView() 
    : closing_(false)
    , observer_manager_(this) {
  root_ = this;
  default_theme_provider_.reset(new ui::DefaultThemeProvider);
}

RootView::~RootView() {
  window_.reset();
  host_->RemoveObserver(this);
  focus_client_.reset();
  host_.reset();
}

void RootView::Init(const InitParams& params) {
  host_.reset(aura::WindowTreeHost::Create(params.bounds));
  host_->window()->SetBounds(gfx::Rect(params.bounds.size()));
  focus_client_.reset(new FocusClient);
  aura::client::SetFocusClient(host_->window(), focus_client_.get());

  event_client_.reset(new EventClient(this));
  aura::client::SetEventClient(host_->window(), event_client_.get());

  window_.reset(new aura::Window(NULL));
  window()->Init(aura::WINDOW_LAYER_NOT_DRAWN);
  window()->set_id(ViewsIDAllocator::Pointer()->allocate_id());
  window()->SetBounds(gfx::Rect(params.bounds.size()));
  window()->SetName("ContentWindowContainer");
  window()->Show();

  host_->InitHost();
  host_->window()->AddChild(window());
  host_->AddObserver(this);

  observer_manager_.Add(GetNativeTheme());
}

void RootView::Close() {
  closing_ = true;
}

void RootView::Show() {
  host_->Show();
}

void RootView::Hide() {
  host_->Hide();
}

ui::ThemeProvider* RootView::GetThemeProvider() const {
  return default_theme_provider_.get();
}

const ui::NativeTheme* RootView::GetNativeTheme() const {
  return ui::NativeThemeAura::instance();
}

void RootView::OnNativeThemeUpdated(ui::NativeTheme* observed_theme) {
  DCHECK(observer_manager_.IsObserving(observed_theme));

  ui::NativeTheme* current_native_theme = GetNativeTheme();
  if (!observer_manager_.IsObserving(current_native_theme)) {
    observer_manager_.RemoveAll();
    observer_manager_.Add(current_native_theme);
  }

  root_->PropagateNativeThemeChanged(current_native_theme);
}
}  // namespace views
