#include "azer/render_system/d3d11/plugin.h"
#include "azer/render_system/d3d11/d3denv.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/azer_d3denv.h"
#include "azer/render_system/d3d11/angle_d3denv.h"

using azer::d3d11::D3DEnvironment;
using azer::d3d11::D3DEnvironmentPtr;
using azer::d3d11::D3DRenderSystem;
using azer::d3d11::InternalD3DEnvironment;
using azer::d3d11::AngleD3DEnvironment;

extern "C" {
azer::RenderSystem* CreateRenderSystem() {
  D3DEnvironmentPtr envptr(new InternalD3DEnvironment());
  if (!envptr->Initialize()) {
    LOG(ERROR) << "Failed to inititialize D3D11Environment";
    return NULL;
  }
  /*
  D3DEnvironmentPtr envptr(new AngleD3DEnvironment(sur));
  if (!envptr->Initialize()) {
    LOG(ERROR) << "Failed to inititialize D3D11Environment";
    return NULL;
  }
  */

  std::unique_ptr<D3DRenderSystem> rs(new D3DRenderSystem(envptr));
  if (rs->Init()) {
    return rs.release();
  } else {
    return NULL;
  }
}

void ReleaseRenderSystem(azer::RenderSystem* rs) {
  delete rs;
}
}  // extern "C"
