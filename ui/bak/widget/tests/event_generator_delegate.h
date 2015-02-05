#pragma once

#include "azer/ui/widget/widget.h"
#include "azer/ui/widget/widget_tree_host.h"
#include "azer/ui/widget/tests/event_generator.h"
#include "azer/ui/widget/client/screen_position_client.h"


namespace azer {
namespace widget {
namespace testing {

void InitializeAuraEventGeneratorDelegate();

// Implementation of ui::test::EventGeneratorDelegate for Aura.
class EventGeneratorDelegateAura : public EventGeneratorDelegate {
 public:
  EventGeneratorDelegateAura();
  ~EventGeneratorDelegateAura() override;

  // Returns the host for given point.
  virtual WidgetTreeHost* GetHostAt(const gfx::Point& point) const = 0;

  // Returns the screen position client that determines the
  // coordinates used in EventGenerator. EventGenerator uses
  // root Widget's coordinate if this returns NULL.
  virtual client::ScreenPositionClient* GetScreenPositionClient(
      const Widget* window) const = 0;

  // Overridden from ui::test::EventGeneratorDelegate:
  ui::EventTarget* GetTargetAt(const gfx::Point& location) override;
  ui::EventSource* GetEventSource(ui::EventTarget* target) override;
  gfx::Point CenterOfTarget(const ui::EventTarget* target) const override;
  gfx::Point CenterOfWindow(Widget* window) const override;
  void ConvertPointFromTarget(const ui::EventTarget* target,
                              gfx::Point* point) const override;
  void ConvertPointToTarget(const ui::EventTarget* target,
                            gfx::Point* point) const override;
  void ConvertPointFromHost(const ui::EventTarget* hosted_target,
                            gfx::Point* point) const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(EventGeneratorDelegateAura);
};

}  // namespace testing
}  // namespace widget
}  // namespace azer


