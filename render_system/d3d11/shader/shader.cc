#include "azer/render_system/d3d11/plugin.h"

#include "azer/render/render_system.h"
#include "azer/render/util/coordinate_grid_effect.h"
#include "azer/render_system/d3d11/shader/coordinate_effect.h"

extern "C" {
const char* AzerGetShaderProgram(const char* name, const char* version, 
                                 int32 stage, const void* extra) {
  if (strcmp(name, azer::CoordinateGridEffect::kEffectName) == 0) {
    if (stage == azer::kVertexStage) {
      return azer::d3d11::coordinate_effect_hlsl_vs;
    } else if (stage == azer::kPixelStage) {
      return azer::d3d11::coordinate_effect_hlsl_ps;
    } else {
      return NULL;
    }
  }

  return NULL;
}

}  // namespace "C"
