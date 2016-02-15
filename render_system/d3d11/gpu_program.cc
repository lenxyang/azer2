#include "azer/render_system/d3d11/gpu_program.h"

#include "azer/base/string.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/vertex_buffer.h"

namespace azer {
namespace d3d11 {
D3DVertexGpuProgram::D3DVertexGpuProgram(VertexDescPtr desc, const StageShader& info)
    : VertexGpuProgram(desc, info)
    , resource_(NULL) {
}

D3DVertexGpuProgram::~D3DVertexGpuProgram() { SAFE_RELEASE(resource_);}
bool D3DVertexGpuProgram::Init(RenderSystem* vrs) {
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
D3DGpuProgram::D3DGpuProgram(const StageShader& info) : GpuProgram(info) {}
D3DGpuProgram::~D3DGpuProgram() {}
D3DBlobPtr D3DGpuProgram::CompileShader(ID3D11Device* d3ddevice) {
  std::string msg;
  D3DBlobPtr blob(CompileShaderForStage(stage(), info_.code, info_.path, &msg));
  if (NULL == blob) {
    LOG(ERROR) << "Failed to shader("  << RenderStageName(stage()) << "): " << msg;
    return D3DBlobPtr();
  }
  return blob;
}

bool D3DGpuProgram::Init(RenderSystem* vrs) {
  D3DRenderSystem* rs = (D3DRenderSystem*)vrs;
  ID3D11Device* d3ddevice = rs->GetDevice();
  D3DBlobPtr blob = CompileShader(d3ddevice);
  if (!blob.get()) {
    return false;
  }
  return InitResource(d3ddevice, blob.get());
}

// class 
D3DPixelGpuProgram::D3DPixelGpuProgram(const StageShader& info)
    : D3DGpuProgram(info), resource_(NULL) {}
D3DPixelGpuProgram::~D3DPixelGpuProgram() { SAFE_RELEASE(resource_);}
bool D3DPixelGpuProgram::InitResource(ID3D11Device* d3ddevice, ID3DBlob* blob) {
  DCHECK(NULL == resource_);
  HRESULT hr = d3ddevice->CreatePixelShader(blob->GetBufferPointer(),
                                            blob->GetBufferSize(),
                                            NULL,
                                            &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreatePixelShader failed ");
  return true;
}

D3DGeometryGpuProgram::D3DGeometryGpuProgram(const StageShader& info) 
    : D3DGpuProgram(info), resource_(NULL) {}
D3DGeometryGpuProgram::~D3DGeometryGpuProgram() {SAFE_RELEASE(resource_);}
bool D3DGeometryGpuProgram::InitResource(ID3D11Device* d3ddevice, ID3DBlob* blob) {
  HRESULT hr = d3ddevice->CreateGeometryShader(blob->GetBufferPointer(),
                                               blob->GetBufferSize(),
                                               NULL,
                                               &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreateGeometryShader failed ");
  return true;
}

D3DHullGpuProgram::D3DHullGpuProgram(const StageShader& info) 
    : D3DGpuProgram(info), resource_(NULL) {}
D3DHullGpuProgram::~D3DHullGpuProgram() {SAFE_RELEASE(resource_);}
bool D3DHullGpuProgram::InitResource(ID3D11Device* d3ddevice, ID3DBlob* blob) {
  HRESULT hr = d3ddevice->CreateHullShader(blob->GetBufferPointer(),
                                           blob->GetBufferSize(),
                                           NULL,
                                           &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreateHullShader failed ");
  return true;
}

D3DDomainGpuProgram::D3DDomainGpuProgram(const StageShader& info) 
    : D3DGpuProgram(info), resource_(NULL) {}
D3DDomainGpuProgram::~D3DDomainGpuProgram() {SAFE_RELEASE(resource_);}
bool D3DDomainGpuProgram::InitResource(ID3D11Device* d3ddevice, ID3DBlob* blob) {
  HRESULT hr = d3ddevice->CreateDomainShader(blob->GetBufferPointer(),
                                             blob->GetBufferSize(),
                                             NULL,
                                             &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreateDomainShader failed ");
  return true;
}

D3DComputeGpuProgram::D3DComputeGpuProgram(const StageShader& info) 
    : D3DGpuProgram(info), resource_(NULL) {}
D3DComputeGpuProgram::~D3DComputeGpuProgram() {SAFE_RELEASE(resource_);}
bool D3DComputeGpuProgram::InitResource(ID3D11Device* d3ddevice, ID3DBlob* blob) {
  HRESULT hr = d3ddevice->CreateComputeShader(blob->GetBufferPointer(),
                                              blob->GetBufferSize(),
                                              NULL,
                                              &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreateComputeShader failed ");
  return true;
}
}  // namespace d3d11
}  // namespace azer
