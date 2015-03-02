#pragma once

#include <vector>

#include "azer/ui/views/view.h"
#include "azer/ui/views/focus/focus_manager.h"
#include "azer/ui/views/focus/focus_search.h"

namespace views {
class Widget;

namespace internal {
class VIEWS_EXPORT RootView : public View,
                              public FocusTraversable {
 public:
  RootView(Widget* widget);
  virtual ~RootView();

  void Init(const gfx::Rect& bounds);
  const char* GetClassName() const override;

  // Sets the specified view as the contents of this Widget. There can only
  // be one contents view child of this Widget's RootView. This view is sized to
  // fit the entire size of the RootView. The RootView takes ownership of this
  // View, unless it is set as not being parent-owned.
  void SetContentsView(View* view);
  View* GetContentsView();
 private:
  void OnBoundsChanged(const gfx::Rect& previous_bounds, 
                       const gfx::Rect& new_bounds) override;
  Widget* widget_;
  View* contents_view_;
  friend class View;
  DISALLOW_COPY_AND_ASSIGN(RootView);
};
}  // namespace internal
}  // namespace views
