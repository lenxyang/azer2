#pragma once

#include "ui/events/event_targeter.h" 
#include "azer/base/export.h"

namespace azer {
namespace widget {

class Widget;

class AZER_EXPORT WidgetTargeter : public ui::EventTargeter {
 public:
  WidgetTargeter();
  ~WidgetTargeter() override;
 protected:
  // ui::EventTargeter:
  ui::EventTarget* FindTargetForEvent(ui::EventTarget* root,
                                      ui::Event* event) override;
  ui::EventTarget* FindTargetForLocatedEvent(ui::EventTarget* root,
                                             ui::LocatedEvent* event) override;
  bool SubtreeCanAcceptEvent(ui::EventTarget* target,
                             const ui::LocatedEvent& event) const override;
  bool EventLocationInsideBounds(ui::EventTarget* target,
                                 const ui::LocatedEvent& event) const override;
 protected:
  DISALLOW_COPY_AND_ASSIGN(WidgetTargeter);
};

}  // namespace widget
}  // namespace azer

