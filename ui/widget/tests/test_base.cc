#include "azer/ui/widget/tests/test_base.h"

#include "base/logging.h"
#include "base/run_loop.h"
#include "ui/events/event.h"
#include "ui/events/event_dispatcher.h"
#include "ui/events/event_processor.h"

#include "azer/ui/widget/api.h"
#include "azer/ui/widget/client/default_capture_client.h"

namespace azer {
namespace widget {
namespace testing {

WidgetTestBase::WidgetTestBase() 
    : first_frame_rendered_(false) {
  InitializeAuraEventGeneratorDelegate();
}

WidgetTestBase::~WidgetTestBase() {
}

void WidgetTestBase::SetUp() {
  first_frame_rendered_ = false;
  WidgetContext::Init();
  host_.reset(WidgetTreeHost::Create(gfx::Rect(100, 100, 800, 600)));
  host_->Show();

  capture_client_.reset(new client::DefaultCaptureClient(root_widget()));
  render_loop_.reset(new RenderLoop(this));
}

void WidgetTestBase::TearDown() {
  WidgetContext::Destroy();
}

Widget* WidgetTestBase::root_widget() {
  return host_->widget();
}

bool WidgetTestBase::RunFirstFrame() {
  DCHECK(host_.get());
  DCHECK(render_loop_.get());
  DCHECK(!first_frame_rendered_);
  first_frame_rendered_ = true;
  return render_loop_->RunFirstFrame(host_.get());
}
bool WidgetTestBase::RunNextFrame() {
  DCHECK(host_.get());
  DCHECK(render_loop_.get());
  DCHECK(first_frame_rendered_);
  return render_loop_->RunNextFrame(host_.get());
}

Widget* WidgetTestBase::CreateWidget(WidgetDelegate* delegate,
                                     const gfx::Rect& bounds) {
  return CreateWidget(delegate, bounds, root_widget());
}

Widget* WidgetTestBase::CreateWidget(WidgetDelegate* delegate,
                                     const gfx::Rect& bounds, Widget* parent) {
  return CreateWidget(delegate, bounds, -1, parent);
}

Widget* WidgetTestBase::CreateWidget(WidgetDelegate* delegate, int id, 
                                     const gfx::Rect& bounds, Widget* parent) {
  Widget* widget = new Widget(Widget::kCanvas, parent, id);
  widget->SetBounds(bounds);
  widget->SetDelegate(delegate);
  return widget;
}

Widget* WidgetTestBase::CreateWidget(const gfx::Rect& bounds, Widget* parent) {
  return CreateWidget(NULL, bounds, parent);
}

bool WidgetTestBase::DispatchEventUsingWidgetDispatcher(ui::Event* event) {
  CHECK(host_.get());
  ui::EventProcessor* processor = host_->event_processor();
  CHECK(processor);
  ui::EventDispatchDetails details = processor->OnEventFromSource(event);
  CHECK(!details.dispatcher_destroyed);
  return event->handled();
}
}  // namespace testing
}  // namespace widget
}  // namespace azer
