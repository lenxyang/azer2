#include "azer/ui/views/widget/root_view.h"


#include "ui/native_theme/native_theme_aura.h"
#include "ui/base/default_theme_provider.h"
#include "ui/base/hit_test.h"
#include "ui/base/l10n/l10n_font_util.h"
#include "ui/base/resource/resource_bundle.h"

#include "azer/ui/aura/window_tree_host.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/views/widget/widget.h"

namespace views {
namespace internal {

RootView::RootView(Widget* widget) 
    : widget_(widget)
    , contents_view_(NULL) {
  View::root_ = this;
}

RootView::~RootView() {
}

void RootView::Init(const gfx::Rect& bounds) {
  InitAuraWindow(aura::WINDOW_LAYER_NOT_DRAWN);
  window()->SetBounds(gfx::Rect(bounds.size()));
}

const char* RootView::GetClassName() const {
  return "RootView";
}

void RootView::OnBoundsChanged(const gfx::Rect& previous_bounds,
                               const gfx::Rect& new_bounds) {
  widget_->OnRootViewSetBoundsChanged(new_bounds);
  bounds_ = gfx::Rect(new_bounds.size());
}

void RootView::SetContentsView(View* contents_view) {
  if (has_children()) {
    RemoveAllChildViews();
  }
  AddChildView(contents_view);
  contents_view_ = contents_view;
}

View* RootView::GetContentsView() {
  return contents_view_;
}
}  // namespace internal
}  // namespace views
