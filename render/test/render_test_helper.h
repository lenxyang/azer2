#pragma once


#include "base/callback.h"
#include "base/time/time.h"

namespace azer {

class RenderSystem;

/**
 * 此类帮助创建某人窗口，加载 RenderSystem
 * 此后用户可以进行任意渲染或者销毁 RenderSystem
 *
 */
class RenderTestHelper {
 public:
  RenderTestHelper();
  ~RenderTestHelper();

  void RunOneFrame();
 private:
  RenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(RenderTestHelper);
};
}  // namespace azer
