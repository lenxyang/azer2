#include "azer/render_system/d3d11/plugin.h"
#include "azer/render_system/d3d11/d3denv.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/ui/window/window_host.h"

extern "C" {
azer::RenderSystem* CreateRenderSystem(azer::Surface* sur) {
  azer::D3D11EnvironmentPtr envptr = azer::D3D11Environment::Create("internal", sur);
  if (envptr.get() == NULL) {
    LOG(ERROR) << "Failed to inititialize D3D11Environment";
    return NULL;
  }

  std::unique_ptr<azer::D3D11RenderSystem> rs(new azer::D3D11RenderSystem(envptr));
  if (rs->Init()) {
    rs->GetDefaultRenderer()->SetViewport(azer::Renderer::Viewport());
    return rs.release();
  } else {
    return NULL;
  }
}
}  // extern "C"
