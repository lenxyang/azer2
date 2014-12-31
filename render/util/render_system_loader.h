#pragma once

#include "azer/base/export.h"
#include "azer/render/util/dynlib.h"
#include "azer/render/surface.h"


namespace azer {

class WindowHost;
class RenderSystem;

class AZER_EXPORT AutoRenderSystemInit {
 public:
  AutoRenderSystemInit(const base::FilePath& path, SurfacePtr surface);
  ~AutoRenderSystemInit();

  RenderSystem* GetRenderSystem() { return current_; }
 private:
  bool Init(SurfacePtr surface);
  ::azer::Dynlib dynlib_;
  RenderSystem* current_;
  SurfacePtr surface_;
  DISALLOW_COPY_AND_ASSIGN(AutoRenderSystemInit);
};

bool AZER_EXPORT LoadRenderSystem(gfx::AcceleratedWidget window);
}  // namespace azer
