#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "testing/gtest/include/gtest/gtest.h"

#include "azer/ui/widget/api.h"
#include "azer/ui/widget/client/default_capture_client.h"
#include "azer/ui/widget/tests/event_generator_delegate.h"

namespace azer {
namespace widget {

class WidgetTreeHost;

namespace testing {

class FakeRenderLoopDelegate : public RenderLoopDelegate {
 public:
  virtual bool Initialize(RenderLoop* renderer) { return true;}
  virtual void OnUpdate(const ::base::Time& Time,
                        const ::base::TimeDelta& delta) {}
  virtual void OnRender(const ::base::Time& Time,
                        const ::base::TimeDelta& delta) {}
};

class WidgetTestBase : public ::testing::Test
                     , public FakeRenderLoopDelegate {
 public:
  WidgetTestBase();
  ~WidgetTestBase() override;

  void SetUp() override;
  void TearDown() override;
 protected:
  Widget* root_widget();
  WidgetTreeHost* host() { return host_.get();}

  bool RunFirstFrame();
  bool RunNextFrame();
  Widget* CreateWidget(const gfx::Rect& bounds, WidgetDelegate* delegate);
  Widget* CreateWidget(const gfx::Rect& bounds, Widget* parent, 
                       WidgetDelegate* delegate);

  bool DispatchEventUsingWidgetDispatcher(ui::Event* event);

  base::MessageLoopForUI message_loop_;
  scoped_ptr<WidgetTreeHost> host_;
  scoped_ptr<RenderLoop> render_loop_;
  scoped_ptr<client::DefaultCaptureClient> capture_client_;
  bool first_frame_rendered_;
  DISALLOW_COPY_AND_ASSIGN(WidgetTestBase);
};
}  // namespace testing
}  // namespace widget
}  // namespace azer
