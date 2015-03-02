// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_WIDGET_REMOVALS_OBSERVER_H_
#define UI_VIEWS_WIDGET_WIDGET_REMOVALS_OBSERVER_H_

#include "azer/ui/views/views_export.h"

namespace views {

class Widget;
class View;

// |WidgetRemovalsObserver| complements |WidgetObserver| with additional
// notifications. These include events occurring during tear down like view
// removal. For this reason, it is recommended that subclasses not also inherit
// from |View|.
class VIEWS_EXPORT WidgetRemovalsObserver {
 public:
  // Called immediately before a descendant view of |widget| is removed.
  virtual void OnWillRemoveView(Widget* widget, View* view) {}

 protected:
  virtual ~WidgetRemovalsObserver() {}
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_WIDGET_REMOVALS_OBSERVER_H_
