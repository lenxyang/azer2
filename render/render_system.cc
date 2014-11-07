#include "azer/render/render_system.h"

#include <memory>
#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/math/math.h"
#include "azer/ui/window/window_host.h"
#include "azer/render/util/dynlib.h"
#include "azer/render/skia/context.h"

typedef azer::RenderSystem* (*CreateRenderSystemFunc)(azer::WindowHost* win);

namespace azer {
class AutoRenderSystemInit {
 public:
  AutoRenderSystemInit(const base::FilePath& path, WindowHost* host)
      : dynlib_(path, true)
      , current_(NULL) {
    Init(host);
  }

  ~AutoRenderSystemInit() {
    delete current_;
  }

  RenderSystem* GetRenderSystem() { return current_; }
 private:
  bool Init(WindowHost* win) {
    if(!dynlib_.load()) {
      return false;
    }

    CreateRenderSystemFunc func = (CreateRenderSystemFunc)
        dynlib_.GetSymbol("CreateRenderSystem");
    if (func != NULL && (current_ = (*func)(win))) {
      LOG(ERROR) << "RenderSystem(" << current_->name() << ") Created";
      if (current_) {
        current_->win_host_ = win;
        Renderer* renderer = current_->GetDefaultRenderer();
        DCHECK(renderer->GetFrontFace() == azer::kCounterClockwise);
        DCHECK(renderer->GetCullingMode() == azer::kCullBack);
      }

      win->SetRenderSystem(current_);
      return true;
    } else {
      PLOG(ERROR) << "not a RenderSystem shared library.";
      return false;
    }
  }

  ::azer::Dynlib dynlib_;
  RenderSystem* current_;
};

namespace {
AutoRenderSystemInit *s_render_system_env = NULL;
}  // namespace

RenderSystem* RenderSystem::Current() {
  if (s_render_system_env) {
    return s_render_system_env->GetRenderSystem();
  } else {
    return NULL;
  }
}

RenderSystem* RenderSystem::InitRenderSystem(const ::base::FilePath& path,
                                             WindowHost* win) {
  DCHECK(NULL == s_render_system_env);
  s_render_system_env = new AutoRenderSystemInit(path, win);
  return Current();
}

void RenderSystem::Release() {
  DCHECK(NULL != s_render_system_env);
  delete s_render_system_env;
  s_render_system_env = NULL;
}

skia::Context* RenderSystem::GetSkiaContext() {
  static EGL* egl = CreateEGL();
  std::unique_ptr<skia::Context> ctx(new skia::Context());
  if (ctx->Init(this)) {
    return ctx.release();;
  } else {
    return NULL;
  }
}
}  // namespace azer
