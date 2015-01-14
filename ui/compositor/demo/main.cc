#include "base/message_loop/message_loop.h"

#include "azer/ui/win/context.h"
#include "azer/ui/win/window_tree_host.h"
#include "azer/render/util/render_window.h"

class RenderFrame : public RenderLoop::Delegate {
 public:
  RenderFrame() {}
  ~RenderFrame() {}

  bool Initialize(RenderLoop* renderer) override {
  }
  void OnUpdate(const ::base::Time& Time,
                const ::base::TimeDelta& delta) override {
  }
  void OnRender(const ::base::Time& Time,
                const ::base::TimeDelta& delta) override {
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(RenderFrame);
};

int main(int argc, char* argv[]) {
  ::azer::InitApp(&argc, &argv);
  azer::RenderWindow window;
  window.Loop(delegate);
  return 0;
}
