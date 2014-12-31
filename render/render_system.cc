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

#include "ui/gl/gl_surface.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_share_group.h"
#include "ui/gl/gl_bindings_skia_in_process.h"


namespace azer {

namespace {
class CGLEnvironment {
 public:
  CGLEnvironment()
      : initialized_failed_(false)
      , initialized_(false) {
  }

  bool Init() {
    if (initialized_failed_) { return false;}
    if (initialized_) { return true;}

    gfx::GLSurface::InitializeOneOff();
    surface_ = gfx::GLSurface::CreateOffscreenGLSurface(gfx::Size(1,1));
    if (!surface_.get()) {
      initialized_failed_= true;
      return false;
    }

    gl_share_group_ = new gfx::GLShareGroup;
    context_ = gfx::GLContext::CreateGLContext(
        gl_share_group_.get(), surface_.get(), gfx::PreferDiscreteGpu);
    context_->MakeCurrent(surface_.get());
    initialized_ = true;
    return true;
  }
 private:
  bool initialized_failed_;
  bool initialized_;
  scoped_refptr<gfx::GLShareGroup> gl_share_group_;
  scoped_refptr<gfx::GLContext> context_;
  scoped_refptr<gfx::GLSurface> surface_;
  DISALLOW_COPY_AND_ASSIGN(CGLEnvironment);
};

base::LazyInstance<CGLEnvironment>::Leaky gl_context_ = LAZY_INSTANCE_INITIALIZER;
}

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
  CHECK(gl_context_.Pointer()->Init());
  InitContext2D();
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::InitContext2D() {
  GrGLInterface* interface = gfx::CreateInProcessSkiaGLBinding();
  CHECK(NULL != interface);
  context2d_.reset(new Context2D(interface));
  CHECK(context2d_->Init(this));
}
}  // namespace azer
