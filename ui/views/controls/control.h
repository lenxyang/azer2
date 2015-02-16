#pragma once

#include "azer/ui/widget/widget.h"

namespace widget {
class WIDGET_EXPORT Control : public Widget {
 public:
  Control();
  ~Control() override;
  // Set whether this view is enabled. A disabled view does not receive keyboard
  // or mouse inputs. If |enabled| differs from the current value, SchedulePaint
  // is invoked. Also, clears focus if the focused view is disabled.
  void SetEnabled(bool enabled);

  // Returns whether the view is enabled.
  bool enabled() const { return enabled_; }

  void Layout() override {}

  void SetGroup(int gid);
  int GetGroup() const;
  void GetViewsInGroup(int group, Views* views);
  virtual View* GetSelectedViewForGroup(int group);
 private:
  DISALLOW_COPY_AND_ASSIGN(Control);
};
}  // namespace widget
