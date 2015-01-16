#pragma once

#include <atomic>

#include "base/basictypes.h"
#include "ui/events/event_target.h"

#include "azer/base/export.h"

namespace azer {
namespace widget {

class AZER_EXPORT WidgetContext : public ::ui::EventTarget {
 public:
  static void Init();
  static WidgetContext* GetInstance();
  static void Destroy();

  int64 allocate_widget_id();
 protected:
  WidgetContext();
  virtual ~WidgetContext();

  // Overridden from ui::EventTarget:
  bool CanAcceptEvent(const ui::Event& event) override;
  ui::EventTarget* GetParentTarget() override;
  scoped_ptr<ui::EventTargetIterator> GetChildIterator() const override;
  ui::EventTargeter* GetEventTargeter() override;

  int64 allocated_widget_id_;
  static WidgetContext* inst_;
  DISALLOW_COPY_AND_ASSIGN(WidgetContext);
};

}  // namespace widget
}  // namespace azer

