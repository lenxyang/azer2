#pragma once

#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "azer/base/export.h"
#include "azer/ui/widget/widget_observer.h"

namespace azer {
namespace widget {

class Widget;

// This class keeps track of a set of widgets. A Widget is removed either
// explicitly by Remove(), or implicitly when the widget is destroyed.
class AZER_EXPORT WidgetTracker : public WidgetObserver {
 public:
  typedef std::set<Widget*> Widgets;

  WidgetTracker();
  ~WidgetTracker() override;

  // Returns the set of widgets being observed.
  const std::set<Widget*>& widgets() const { return widgets_; }

  // Adds |widget| to the set of Widgets being tracked.
  void Add(Widget* widget);

  // Removes |widget| from the set of widgets being tracked.
  void Remove(Widget* widget);

  // Returns true if |widget| was previously added and has not been removed or
  // deleted.
  bool Contains(Widget* widget);

  // WidgetObserver overrides:
  void OnWidgetDestroying(Widget* widget) override;

 private:
  Widgets widgets_;

  DISALLOW_COPY_AND_ASSIGN(WidgetTracker);
};

}  // namespace widget
}  // namespace azer

