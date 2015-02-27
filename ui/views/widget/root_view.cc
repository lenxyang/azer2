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
    : widget_(widget) {
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
  widget_->SetBounds(new_bounds);
  bounds_ = gfx::Rect(new_bounds.size());
}

}  // namespace internal
}  // namespace views
