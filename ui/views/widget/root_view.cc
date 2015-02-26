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

RootView::RootView(Widget* widget) 
    : root_(this)
    , widget_(widget) {
}

RootView::~RootView() {
}

}  // namespace views
