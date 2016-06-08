#pragma once

#include <string>

#include "base/logging.h"
#include "azer/render/rasterizer_state.h"
#include "azer/render/shader.h"
#include "azer/render_system/d3d11/d3d11.h"

namespace azer {
namespace d3d11 {
class D3DRasterizerState : public RasterizerState {
 public:
  D3DRasterizerState();
  ~D3DRasterizerState() override;

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
  ID3D11RasterizerState* raster_state_;
  DISALLOW_COPY_AND_ASSIGN(D3DRasterizerState);
};
}  // namespace d3d11
}  // namespace azer
