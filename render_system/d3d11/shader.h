#pragma once

#include <string>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/render/shader.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/hlsl_compile.h"

namespace azer {
namespace d3d11 {

class D3DRenderSystem;
class D3DVertexShader : public VertexShader {
 public:
  D3DVertexShader(VertexDescPtr desc, const ShaderInfo& info);
  ~D3DVertexShader() override;
  bool Init(RenderSystem* rs) override;
  ID3D11VertexShader* resource() { return resource_;}
 private:
  ID3D11VertexShader* resource_;

  friend class D3DRenderer;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DVertexShader);
};

class D3DShader : public Shader {
 public:
  D3DShader(const ShaderInfo& info);
  ~D3DShader();

  bool Init(RenderSystem* rs) override;
 private:
  D3DBlobPtr CompileShader(ID3D11Device* d3d_device);
  virtual bool InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) = 0;
  DISALLOW_COPY_AND_ASSIGN(D3DShader);
};

class D3DPixelShader : public D3DShader {
 public:
  D3DPixelShader(const ShaderInfo& info);
  ~D3DPixelShader() override;
  bool InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) override;
  ID3D11PixelShader* resource() { return resource_;}
 private:
  ID3D11PixelShader* resource_;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DPixelShader);
};

class D3DGeometryShader : public D3DShader {
 public:
  D3DGeometryShader(const ShaderInfo& info);
  virtual ~D3DGeometryShader();
  bool InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) override;
  ID3D11GeometryShader* resource() { return resource_;}
 private:
  ID3D11GeometryShader* resource_;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DGeometryShader);
};

class D3DHullShader : public D3DShader {
 public:
  D3DHullShader(const ShaderInfo& info);
  ~D3DHullShader() override;
  bool InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) override;
  ID3D11HullShader* resource() { return resource_;}
 private:
  ID3D11HullShader* resource_;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DHullShader);
};

class D3DDomainShader : public D3DShader {
 public:
  D3DDomainShader(const ShaderInfo& info);
  ~D3DDomainShader() override;
  bool InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) override;
  ID3D11DomainShader* resource() { return resource_;}
 private:
  ID3D11DomainShader* resource_;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DDomainShader);
};

class D3DComputeShader : public D3DShader {
 public:
  D3DComputeShader(const ShaderInfo& info);
  ~D3DComputeShader() override;
  bool InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) override;
  ID3D11ComputeShader* resource() { return resource_;}
 private:
  ID3D11ComputeShader* resource_;
  friend class D3DTechnique;
  DISALLOW_COPY_AND_ASSIGN(D3DComputeShader);
};

}  // namespace d3d11
}  // namespace azer
