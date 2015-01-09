#include "azer/render/util/render_system_loader.h"

#include "base/files/file_path.h"
#include "base/command_line.h"
#include "base/lazy_instance.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/render/render.h"
#include "azer/render/util/dynlib.h"

namespace azer {

namespace {
typedef azer::RenderSystem* (*CreateRenderSystemFunc)(azer::Surface* surface);

AutoRenderSystemInit *s_render_system_env = NULL;
}  // namespace

AutoRenderSystemInit::AutoRenderSystemInit(const base::FilePath& path,
                                           SurfacePtr surface)
    : dynlib_(path, true)
    , current_(NULL)
    , surface_(surface) {
  Init(surface_);
  RenderSystem::SetRenderSystem(current_);
}

AutoRenderSystemInit:: ~AutoRenderSystemInit() {
  RenderSystem::SetRenderSystem(NULL);
  delete current_;
}

bool AutoRenderSystemInit::Init(SurfacePtr surface) {
  if(!dynlib_.load()) {
    return false;
  }

  CreateRenderSystemFunc func = (CreateRenderSystemFunc)
      dynlib_.GetSymbol("CreateRenderSystem");
  if (func != NULL && (current_ = (*func)(surface.get()))) {
    LOG(ERROR) << "RenderSystem(" << current_->name() << ") Created";
    return true;
  } else {
    PLOG(ERROR) << "not a RenderSystem shared library.";
    return false;
  }
}

bool LoadRenderSystem(gfx::AcceleratedWidget window) {
  DCHECK(RenderSystem::Current() == NULL);
  DCHECK(NULL != CommandLine::ForCurrentProcess());
  ::base::FilePath path = CommandLine::ForCurrentProcess()->GetProgram();
  ::base::FilePath dllpath = path.DirName();
  dllpath = dllpath.Append(::base::UTF8ToWide("d3d11_render_system.dll"));

  SurfacePtr surface(new Surface(window));
  s_render_system_env = new AutoRenderSystemInit(dllpath, surface);
  return s_render_system_env->GetRenderSystem() != NULL;
}
}  // namespace azer
