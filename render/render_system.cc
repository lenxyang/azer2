#include "azer/render/render_system.h"

#include <memory>
#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/math/math.h"
#include "azer/render/surface.h"
#include "azer/render/util/dynlib.h"
#include "azer/render/context2d.h"

#include "ui/gl/gl_surface.h"
#include "ui/gl/gl_bindings_skia_in_process.h"

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
    : surface_(surface) {
  gfx::GLSurface::InitializeOneOff();
}

Context2D* RenderSystem::GetContext2D() {
  GrGLInterface* interface = gfx::CreateInProcessSkiaGLBinding();
  std::unique_ptr<Context2D> ctx(new Context2D(interface));
  if (ctx->Init(this)) {
    return ctx.release();
  } else {
    return NULL;
  }
}
}  // namespace azer
