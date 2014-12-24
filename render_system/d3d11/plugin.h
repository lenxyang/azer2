#pragma once

#include "azer/render_system/d3d11/export.h"
#include "ui/gfx/native_widget_types.h"

namespace azer {
class RenderSystem;
class Surface;
}  // namespace azer

extern "C" AZER_D3D11RS_EXPORT azer::RenderSystem* CreateRenderSystem(azer::Surface*);
    
