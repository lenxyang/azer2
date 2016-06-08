#pragma once

#include "azer/render_system/d3d11/export.h"
#include "ui/gfx/native_widget_types.h"

namespace azer {
class RenderSystem;
}  // namespace azer

extern "C" AZER_D3D11RS_EXPORT azer::RenderSystem* CreateRenderSystem();

extern "C" AZER_D3D11RS_EXPORT void ReleaseRenderSystem(azer::RenderSystem* rs);

extern "C" AZER_D3D11RS_EXPORT const char* AzerGetShaderProgram(
    const char* name, const char* version, int32_t stage, const void* extra);
    
