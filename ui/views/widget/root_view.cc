#pragma once

#include "base/basictypes.h"
#include "azer/ui/views/widget/root_view.h"
#include "azer/ui/views/widget/widget.h"
#include "azer/ui/aura/window.h"

namespace views {
namespace internal {

RootView::RootView(Widget* widget)
    : widget_(widget) {
  window()->SetName("root");
}

RootView::~RootView() {
}

}  // namespace internal
}  // namespace views
