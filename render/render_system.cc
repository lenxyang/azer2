#include "azer/render/render_system.h"


#include "third_party/skia/include/gpu/gl/GrGLInterface.h"

#include <memory>
#include "base/basictypes.h"
#include "base/logging.h"
#include "base/lazy_instance.h"
#include "azer/math/math.h"
#include "azer/render/surface.h"
#include "azer/render/util/dynlib.h"
#include "azer/render/context2d.h"


namespace azer {

RenderSystem* RenderSystem::render_system_ = NULL;

void RenderSystem::SetRenderSystem(RenderSystem* rs) {
  if (rs != NULL) {
    CHECK(NULL == render_system_);
    render_system_ = rs;
  } else {
    render_system_ = rs;
  }
}

RenderSystem* RenderSystem::Current() {
  return render_system_;
}

RenderSystem::RenderSystem(Surface* surface)
    : surface_(surface) 
    , current_renderer_(NULL) {
  capability_ = std::move(ResetCapability());
  context2d_.reset(InitContext2D());
  CHECK(context2d_.get() != NULL) << "Failed to init context2D.";
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::SetSwapchain(SwapChainPtr& swapchain) {
}
}  // namespace azer
