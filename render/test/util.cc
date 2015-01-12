#include "azer/render/test/util.h"

#include "azer/util/util.h"
#include "azer/render/render.h"
#include "azer/sandbox/base/sampleapp.h"

namespace azer {
namespace testing {

class TestWinDelegate : public SampleApp::Delegate {
 public:
  TestWinDelegate() {}
  virtual bool OnInit() {return true;}
  virtual void OnUpdateScene(double time, float delta_time) {}
  virtual void OnRenderScene(double time, float delta_time) {}
  virtual void OnQuit() {}
 private:
  DISALLOW_COPY_AND_ASSIGN(TestWinDelegate);
};

RenderSystem* CreateD3D11RenderSystemForTest() {
  TestWinDelegate delegate;
  SampleApp app(&delegate);
  app.Init();
  return RenderSystem::Current();
}
}  // namespace testing
}  // namespace azer
