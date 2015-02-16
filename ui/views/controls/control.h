#pragma once

#include "base/basictypes.h"
#include "azer/ui/views/view.h"

namespace views {
class VIEWS_EXPORT Control : public View {
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

  void SetGroup(int32 gid);
  int32 GetGroup() const;
  virtual View* GetSelectedViewForGroup(int group);

  virtual void PreferredSizeChanged() {}
 private:
  int32 group_id_;
  bool enabled_;
  DISALLOW_COPY_AND_ASSIGN(Control);
};
}  // namespace views
