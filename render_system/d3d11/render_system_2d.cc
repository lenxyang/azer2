#include "azer/render_system/d3d11/render_system.h"
/*
#include "azer/render_system/d3d11/canvas2d.h"
#include "azer/render_system/d3d11/context2d.h"
#include "azer/render_system/d3d11/angle_env.h"
*/
#include "azer/render/context2d_software.h"

namespace azer {
namespace d3d11 {

bool use_hardware_context = false;

void D3DRenderSystem::InitContext2D() {
  /*
  if (use_hardware_context) {
    AngleEnv::Pointer()->InitForOffscreen();
    std::unique_ptr<D3DContext2D> ptr(new D3DContext2D);
    if (ptr->Init(this)) {
      context2d_.reset(ptr.release());
    }
  } else {
    context2d_.reset(new SoftwareContext2D);
  }
  */
  context2d_.reset(new SoftwareContext2D);
}

}  // namespace d3d11
}  // namespace azer
