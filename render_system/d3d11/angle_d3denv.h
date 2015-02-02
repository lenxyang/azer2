#pragma once

#include "azer/render_system/d3d11/d3denv.h"
#include "azer/render/surface.h"
#include "azer/render/swap_chain.h"

namespace azer {
namespace d3d11 {

class AngleD3DEnvironment : public D3DEnvironment {
 public:
  AngleD3DEnvironment(Surface* surface);

  bool Initialize() override;

  D3DEnvSwapChain* CreateSwapChain(Surface* surface) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(AngleD3DEnvironment);
};

}  // namespace d3d11
}  // namespace azer

