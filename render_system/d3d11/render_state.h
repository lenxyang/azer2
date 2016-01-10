#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

#include <string>

#include "base/logging.h"
#include "azer/render/render_state.h"
#include "azer/render/gpu_program.h"

namespace azer {
namespace d3d11 {
class D3DRenderState : public RenderState {
 public:
  D3DRenderState();
  ~D3DRenderState() override;

  FillMode GetFillMode(void) override;
  void SetFillMode(FillMode mode) override;
  CullingMode GetCullingMode(void) override;
  void SetCullingMode(CullingMode mode) override;
  FrontFace GetFrontFace() override;
  void SetFrontFace(FrontFace mode) override;
  void EnableMultisampleAntiAliasing(bool enable) override;
  bool IsMultisampleAntiAliasingEnabled() override;
  void EnableLineAntialiasing(bool enable) override;
  bool IsLineAntialiasingEnabled() override;

  void Apply(Renderer* renderer) override;
 private:
  void SetRasterizerState(const D3D11_RASTERIZER_DESC& desc);
  ID3D11RasterizerState* state_;
  DISALLOW_COPY_AND_ASSIGN(D3DRenderState);
};
}  // namespace d3d11
}  // namespace azer
