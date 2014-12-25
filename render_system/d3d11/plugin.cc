#include "azer/render_system/d3d11/plugin.h"
#include "azer/render_system/d3d11/d3denv.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/ui/window/window_host.h"

using azer::d3d11::D3DEnvironment;
using azer::d3d11::D3DEnvironmentPtr;
using azer::d3d11::D3DRenderSystem;

extern "C" {
azer::RenderSystem* CreateRenderSystem(azer::Surface* sur) {
  D3DEnvironmentPtr envptr = D3DEnvironment::Create("internal", sur);
  if (envptr.get() == NULL) {
    LOG(ERROR) << "Failed to inititialize D3D11Environment";
    return NULL;
  }

  std::unique_ptr<D3DRenderSystem> rs(new D3DRenderSystem(envptr));
  if (rs->Init()) {
    rs->GetDefaultRenderer()->SetViewport(azer::Renderer::Viewport());
    return rs.release();
  } else {
    return NULL;
  }
}
}  // extern "C"
