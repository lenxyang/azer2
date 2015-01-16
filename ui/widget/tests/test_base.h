#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "testing/gtest/include/gtest/gtest.h"

#include "azer/ui/widget/api.h"

namespace azer {
namespace widget {

class WidgetTreeHost;

class FakeRenderLoopDelegate : public RenderLoopDelegate {
 public:
  virtual bool Initialize(RenderLoop* renderer) { return true;}
  virtual void OnUpdate(const ::base::Time& Time,
                        const ::base::TimeDelta& delta) {}
  virtual void OnRender(const ::base::Time& Time,
                        const ::base::TimeDelta& delta) {}
};

class WidgetTestBase : public testing::Test
                     , public FakeRenderLoopDelegate {
 public:
  WidgetTestBase();
  ~WidgetTestBase() override;

  void SetUp() override;
  void TearDown() override;
 protected:
  Widget* root();
  WidgetTreeHost* host() { return host_.get();}

  base::MessageLoopForUI message_loop_;
  scoped_ptr<WidgetTreeHost> host_;
  scoped_ptr<RenderLoop> render_loop_;
  DISALLOW_COPY_AND_ASSIGN(WidgetTestBase);
};
}  // namespace widget
}  // namespace azer
