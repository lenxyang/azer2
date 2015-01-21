#include "azer/ui/widget/tests/event_generator_delegate.h"

#include "base/memory/singleton.h"

#include "azer/ui/widget/widget_event_dispatcher.h"


namespace azer {
namespace widget {
namespace testing {
namespace {

class DefaultEventGeneratorDelegate : public EventGeneratorDelegateAura {
 public:
  static DefaultEventGeneratorDelegate* GetInstance() {
    return Singleton<DefaultEventGeneratorDelegate>::get();
  }

  // EventGeneratorDelegate:
  void SetContext(EventGenerator* owner,
                  Widget* root_window,
                  Widget* window) override {
    root_window_ = root_window;
  }

  // EventGeneratorDelegateAura:
  WidgetTreeHost* GetHostAt(const gfx::Point& point) const override {
    return root_window_->GetHost();
  }

  client::ScreenPositionClient* GetScreenPositionClient(
      const Widget* window) const override {
    return NULL;
  }

 private:
  friend struct DefaultSingletonTraits<DefaultEventGeneratorDelegate>;

  DefaultEventGeneratorDelegate() : root_window_(NULL) {
    DCHECK(!EventGenerator::default_delegate);
    EventGenerator::default_delegate = this;
  }

  ~DefaultEventGeneratorDelegate() override {
    DCHECK_EQ(this, EventGenerator::default_delegate);
    EventGenerator::default_delegate = NULL;
  }

  Widget* root_window_;

  DISALLOW_COPY_AND_ASSIGN(DefaultEventGeneratorDelegate);
};

const Widget* WidgetFromTarget(const ui::EventTarget* event_target) {
  return static_cast<const Widget*>(event_target);
}

}  // namespace

void InitializeAuraEventGeneratorDelegate() {
  DefaultEventGeneratorDelegate::GetInstance();
}

EventGeneratorDelegateAura::EventGeneratorDelegateAura() {
}

EventGeneratorDelegateAura::~EventGeneratorDelegateAura() {
}

ui::EventTarget* EventGeneratorDelegateAura::GetTargetAt(
    const gfx::Point& location) {
  return GetHostAt(location)->widget();
}

ui::EventSource* EventGeneratorDelegateAura::GetEventSource(
    ui::EventTarget* target) {
  return static_cast<Widget*>(target)->GetHost()->GetEventSource();
}

gfx::Point EventGeneratorDelegateAura::CenterOfTarget(
    const ui::EventTarget* target) const {
  gfx::Point center =
      gfx::Rect(WidgetFromTarget(target)->bounds().size()).CenterPoint();
  ConvertPointFromTarget(target, &center);
  return center;
}

gfx::Point EventGeneratorDelegateAura::CenterOfWindow(Widget* window) const {
  return CenterOfTarget(window);
}

void EventGeneratorDelegateAura::ConvertPointFromTarget(
    const ui::EventTarget* event_target,
    gfx::Point* point) const {
  DCHECK(point);
  const Widget* target = WidgetFromTarget(event_target);
  client::ScreenPositionClient* client = GetScreenPositionClient(target);
  if (client)
    client->ConvertPointToScreen(target, point);
  else
    Widget::ConvertPointToTarget(target, target->GetRootWidget(), point);
}

void EventGeneratorDelegateAura::ConvertPointToTarget(
    const ui::EventTarget* event_target,
    gfx::Point* point) const {
  DCHECK(point);
  const Widget* target = WidgetFromTarget(event_target);
  client::ScreenPositionClient* client = GetScreenPositionClient(target);
  if (client)
    client->ConvertPointFromScreen(target, point);
  else
    Widget::ConvertPointToTarget(target->GetRootWidget(), target, point);
}

void EventGeneratorDelegateAura::ConvertPointFromHost(
    const ui::EventTarget* hosted_target,
    gfx::Point* point) const {
  const Widget* window = WidgetFromTarget(hosted_target);
  window->GetHost()->ConvertPointFromHost(point);
}

}  // namespace testing
}  // namespace widget
}  // namespace azer
