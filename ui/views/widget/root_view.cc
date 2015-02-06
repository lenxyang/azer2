#pragma once

#include "base/basictypes.h"
#include "azer/ui/views/widget/root_view.h"
#include "azer/ui/views/widget/widget.h"

namespace views {

RootView::RootView(Widget* widget)
    : widget_(widget) {
}

RootView::~RootView() {
}

}  // namespace views
