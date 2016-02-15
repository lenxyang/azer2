#include "azer/render_system/d3d11/shader.h"

#include "azer/base/string.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/vertex_buffer.h"

namespace azer {
namespace d3d11 {
D3DVertexShader::D3DVertexShader(VertexDescPtr desc, const ShaderInfo& info)
    : VertexShader(desc, info)
    , resource_(NULL) {
}

D3DVertexShader::~D3DVertexShader() { SAFE_RELEASE(resource_);}
bool D3DVertexShader::Init(RenderSystem* vrs) {
  D3DRenderSystem* rs = (D3DRenderSystem*)vrs;
  std::string msg;
  ID3D11Device* d3d_device = rs->GetDevice();

  // D3DBlobPtr blob(CompilePixelShader(program_, &msg));
  D3DBlobPtr blob(CompileShaderForStage(stage(), info_.code, info_.path, &msg));
  if (NULL == blob) {
    LOG(ERROR) << "Failed to compile vertex shader: " << msg;
    return false;
  }

  DCHECK(NULL == resource_);
  HRESULT hr = d3d_device->CreateVertexShader(blob->GetBufferPointer(),
                                              blob->GetBufferSize(),
                                              NULL,
                                              &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreateVertexShader failed ");

  // check layout is validate for shader
  scoped_refptr<D3DVertexLayout> layout(new D3DVertexLayout(desc_ptr_));
  if (!layout->ValidateShaderLayout(vrs, blob.get())) {
    return false;
  }

  return true;
}

// clsas 
D3DShader::D3DShader(const ShaderInfo& info) : Shader(info) {}
D3DShader::~D3DShader() {}
D3DBlobPtr D3DShader::CompileShader(ID3D11Device* d3ddevice) {
  std::string msg;
  D3DBlobPtr blob(CompileShaderForStage(stage(), info_.code, info_.path, &msg));
  if (NULL == blob) {
    LOG(ERROR) << "Failed to shader("  << RenderStageName(stage()) << "): " << msg;
    return D3DBlobPtr();
  }
  return blob;
}

bool D3DShader::Init(RenderSystem* vrs) {
  D3DRenderSystem* rs = (D3DRenderSystem*)vrs;
  ID3D11Device* d3ddevice = rs->GetDevice();
  D3DBlobPtr blob = CompileShader(d3ddevice);
  if (!blob.get()) {
    return false;
  }
  return InitResource(d3ddevice, blob.get());
}

// class 
D3DPixelShader::D3DPixelShader(const ShaderInfo& info)
    : D3DShader(info), resource_(NULL) {}
D3DPixelShader::~D3DPixelShader() { SAFE_RELEASE(resource_);}
bool D3DPixelShader::InitResource(ID3D11Device* d3ddevice, ID3DBlob* blob) {
  DCHECK(NULL == resource_);
  HRESULT hr = d3ddevice->CreatePixelShader(blob->GetBufferPointer(),
                                            blob->GetBufferSize(),
                                            NULL,
                                            &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreatePixelShader failed ");
  return true;
}

D3DGeometryShader::D3DGeometryShader(const ShaderInfo& info) 
    : D3DShader(info), resource_(NULL) {}
D3DGeometryShader::~D3DGeometryShader() {SAFE_RELEASE(resource_);}
bool D3DGeometryShader::InitResource(ID3D11Device* d3ddevice, ID3DBlob* blob) {
  HRESULT hr = d3ddevice->CreateGeometryShader(blob->GetBufferPointer(),
                                               blob->GetBufferSize(),
                                               NULL,
                                               &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreateGeometryShader failed ");
  return true;
}

D3DHullShader::D3DHullShader(const ShaderInfo& info) 
    : D3DShader(info), resource_(NULL) {}
D3DHullShader::~D3DHullShader() {SAFE_RELEASE(resource_);}
bool D3DHullShader::InitResource(ID3D11Device* d3ddevice, ID3DBlob* blob) {
  HRESULT hr = d3ddevice->CreateHullShader(blob->GetBufferPointer(),
                                           blob->GetBufferSize(),
                                           NULL,
                                           &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreateHullShader failed ");
  return true;
}

D3DDomainShader::D3DDomainShader(const ShaderInfo& info) 
    : D3DShader(info), resource_(NULL) {}
D3DDomainShader::~D3DDomainShader() {SAFE_RELEASE(resource_);}
bool D3DDomainShader::InitResource(ID3D11Device* d3ddevice, ID3DBlob* blob) {
  HRESULT hr = d3ddevice->CreateDomainShader(blob->GetBufferPointer(),
                                             blob->GetBufferSize(),
                                             NULL,
                                             &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreateDomainShader failed ");
  return true;
}

D3DComputeShader::D3DComputeShader(const ShaderInfo& info) 
    : D3DShader(info), resource_(NULL) {}
D3DComputeShader::~D3DComputeShader() {SAFE_RELEASE(resource_);}
bool D3DComputeShader::InitResource(ID3D11Device* d3ddevice, ID3DBlob* blob) {
  HRESULT hr = d3ddevice->CreateComputeShader(blob->GetBufferPointer(),
                                              blob->GetBufferSize(),
                                              NULL,
                                              &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreateComputeShader failed ");
  return true;
}
}  // namespace d3d11
}  // namespace azer
