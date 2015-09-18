#pragma once

#include "azer/base/export.h"
#include "azer/base/dynlib.h"
#include "azer/render/surface.h"


namespace azer {

class RenderSystem;

class AZER_EXPORT AutoRenderSystemInit {
 public:
  AutoRenderSystemInit(const base::FilePath& path);
  ~AutoRenderSystemInit();

  RenderSystem* GetRenderSystem() { return current_; }
 private:
  bool Init();
  ::azer::Dynlib dynlib_;
  RenderSystem* current_;
  DISALLOW_COPY_AND_ASSIGN(AutoRenderSystemInit);
};

bool AZER_EXPORT LoadRenderSystem();
void AZER_EXPORT UnloadRenderSystem();
bool AZER_EXPORT IsRenderSystemLoaded();

AZER_EXPORT const char* GetCommonShaderProgram(const char* name, 
                                               const char* version, 
                                               int32 stage, const void* extra);
}  // namespace azer
