#pragma once

#include <string>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/render/gpu_program.h"
#include "azer/render/vertex_data.h"
#include "azer/render_system/d3d11/util.h"

namespace azer {
namespace d3d11 {

class D3DRenderSystem;
class D3DVertexGpuProgram : public VertexGpuProgram {
 public:
  D3DVertexGpuProgram(VertexDescPtr desc, const std::string& program)
      : VertexGpuProgram(desc, program)
      , shader_(NULL)
      , input_layout_(NULL) {
  }

  virtual ~D3DVertexGpuProgram() {
    SAFE_RELEASE(shader_);
    SAFE_RELEASE(input_layout_);
  }

  virtual bool Init(RenderSystem* rs) override;
 private:
  ID3D11VertexShader* shader_;
  ID3D11InputLayout *input_layout_;

  friend class D3DRenderer;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DVertexGpuProgram);
};

class D3DPixelGpuProgram : public GpuProgram {
 public:
  D3DPixelGpuProgram(const std::string& program)
      : GpuProgram(kPixelStage, program)
      , shader_(NULL) {
  }

  virtual ~D3DPixelGpuProgram() {
    SAFE_RELEASE(shader_);
  }

  virtual bool Init(RenderSystem* rs) override;
 private:
  ID3D11PixelShader* shader_;
  friend class D3DRenderer;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DPixelGpuProgram);
};

class D3DGeometryGpuProgram : public GpuProgram {
 public:
  D3DGeometryGpuProgram(const std::string& program)
      : GpuProgram(kGeometryStage, program)
      , shader_(NULL) {
  }

  virtual ~D3DGeometryGpuProgram() {
    SAFE_RELEASE(shader_);
  }

  virtual bool Init(RenderSystem* rs) override;
 private:
  ID3D11GeometryShader* shader_;

  friend class D3DRenderer;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DGeometryGpuProgram);
};
}  // namespace d3d11
}  // namespace azer
