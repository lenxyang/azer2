#include "azer/render/util/render_system_loader.h"

#include "base/files/file_path.h"
#include "base/command_line.h"
#include "base/lazy_instance.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/base/dynlib.h"
#include "azer/render/render.h"

namespace azer {

namespace {
typedef azer::RenderSystem* (*CreateRenderSystemFunc)();

AutoRenderSystemInit *s_render_system_env = NULL;
}  // namespace

AutoRenderSystemInit::AutoRenderSystemInit(const base::FilePath& path)
    : dynlib_(path, true)
    , current_(NULL) {
  Init();
  RenderSystem::SetRenderSystem(current_);
}

AutoRenderSystemInit:: ~AutoRenderSystemInit() {
  RenderSystem::SetRenderSystem(NULL);
  delete current_;
}

bool AutoRenderSystemInit::Init() {
  if(!dynlib_.load()) {
    return false;
  }

  CreateRenderSystemFunc func = (CreateRenderSystemFunc)
      dynlib_.GetSymbol("CreateRenderSystem");
  if (func != NULL && (current_ = (*func)())) {
    LOG(ERROR) << "RenderSystem(" << current_->name() << ") Created";
    return true;
  } else {
    PLOG(ERROR) << "not a RenderSystem shared library.";
    return false;
  }
}

bool LoadRenderSystem() {
  DCHECK(RenderSystem::Current() == NULL);
  DCHECK(NULL != CommandLine::ForCurrentProcess());
  ::base::FilePath path = CommandLine::ForCurrentProcess()->GetProgram();
  ::base::FilePath dllpath = path.DirName();
  dllpath = dllpath.Append(::base::UTF8ToWide("d3d11_render_system.dll"));

  s_render_system_env = new AutoRenderSystemInit(dllpath);
  return s_render_system_env->GetRenderSystem() != NULL;
}

void UnloadRenderSystem() {
  if (s_render_system_env) {
    delete s_render_system_env;
    s_render_system_env = NULL;
  }
}

bool IsRenderSystemLoaded() {
  return RenderSystem::Current() != NULL;
}
}  // namespace azer
