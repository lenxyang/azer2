#pragma once

#include "azer/render_system/d3d11/d3denv.h"
#include "azer/render/surface.h"

namespace azer {
namespace d3d11 {

class InternalD3DEnvironment : public D3DEnvironment {
 public:
  InternalD3DEnvironment(Surface* surface);
  bool ResetSwapChain() override;
  bool Initialize() override;
  D3DSwapChain* CreateSwapChain(D3DRenderSystem* rs) override;
 private:
  bool InitD3DDevice();
  DISALLOW_COPY_AND_ASSIGN(InternalD3DEnvironment);
};

}  // namespace d3d11
}  // namespace azer
