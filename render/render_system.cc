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
  InitContext2D();
}

enderSystem::~RenderSystem() {
}

void RenderSystem::InitContext2D() {
  GrGLInterface* interface = gfx::CreateInProcessSkiaGLBinding();
  CHECK(NULL != interface);
  context2d_.reset(new Context2D(interface));
  CHECK(context2d_.Init());
}
}  // namespace azer
