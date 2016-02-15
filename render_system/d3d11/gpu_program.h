#pragma once

#include <string>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/render/gpu_program.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/hlsl_compile.h"

namespace azer {
namespace d3d11 {

class D3DRenderSystem;
class D3DVertexGpuProgram : public VertexGpuProgram {
 public:
  D3DVertexGpuProgram(VertexDescPtr desc, const StageShader& info);
  ~D3DVertexGpuProgram() override;
  bool Init(RenderSystem* rs) override;
  ID3D11VertexShader* resource() { return resource_;}
 private:
  ID3D11VertexShader* resource_;

  friend class D3DRenderer;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DVertexGpuProgram);
};

class D3DGpuProgram : public GpuProgram {
 public:
  D3DGpuProgram(const StageShader& info);
  ~D3DGpuProgram();

  bool Init(RenderSystem* rs) override;
 private:
  D3DBlobPtr CompileShader(ID3D11Device* d3d_device);
  virtual bool InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) = 0;
  DISALLOW_COPY_AND_ASSIGN(D3DGpuProgram);
};

class D3DPixelGpuProgram : public D3DGpuProgram {
 public:
  D3DPixelGpuProgram(const StageShader& info);
  ~D3DPixelGpuProgram() override;
  bool InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) override;
  ID3D11PixelShader* resource() { return resource_;}
 private:
  ID3D11PixelShader* resource_;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DPixelGpuProgram);
};

class D3DGeometryGpuProgram : public D3DGpuProgram {
 public:
  D3DGeometryGpuProgram(const StageShader& info);
  virtual ~D3DGeometryGpuProgram();
  bool InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) override;
  ID3D11GeometryShader* resource() { return resource_;}
 private:
  ID3D11GeometryShader* resource_;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DGeometryGpuProgram);
};

class D3DHullGpuProgram : public D3DGpuProgram {
 public:
  D3DHullGpuProgram(const StageShader& info);
  ~D3DHullGpuProgram() override;
  bool InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) override;
  ID3D11HullShader* resource() { return resource_;}
 private:
  ID3D11HullShader* resource_;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DHullGpuProgram);
};

class D3DDomainGpuProgram : public D3DGpuProgram {
 public:
  D3DDomainGpuProgram(const StageShader& info);
  ~D3DDomainGpuProgram() override;
  bool InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) override;
  ID3D11DomainShader* resource() { return resource_;}
 private:
  ID3D11DomainShader* resource_;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DDomainGpuProgram);
};

class D3DComputeGpuProgram : public D3DGpuProgram {
 public:
  D3DComputeGpuProgram(const StageShader& info);
  ~D3DComputeGpuProgram() override;
  bool InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) override;
  ID3D11ComputeShader* resource() { return resource_;}
 private:
  ID3D11ComputeShader* resource_;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DComputeGpuProgram);
};

}  // namespace d3d11
}  // namespace azer
