#include "azer/render_system/d3d11/gpu_program.h"

#include "azer/base/string.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/hlsl_compile.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/vertex_buffer.h"

namespace azer {
namespace d3d11 {

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

  DCHECK(NULL == shader_);
  HRESULT hr = d3d_device->CreateVertexShader(blob->GetBufferPointer(),
                                              blob->GetBufferSize(),
                                              NULL,
                                              &shader_);
  HRESULT_HANDLE(hr, ERROR, "CreateVertexShader failed ");

  // check layout is validate for shader
  scoped_refptr<D3DVertexLayout> layout(new D3DVertexLayout(desc_ptr_));
  if (!layout->Init(vrs, blob.get())) {
    return false;
  }

  return true;
}

bool D3DPixelGpuProgram::Init(RenderSystem* vrs) {
  D3DRenderSystem* rs = (D3DRenderSystem*)vrs;
  std::string msg;
  ID3D11Device* d3d_device = rs->GetDevice();
  D3DBlobPtr blob(CompileShaderForStage(stage(), info_.code, info_.path, &msg));
  if (NULL == blob) {
    LOG(ERROR) << "Failed to compiler pixel shader: " << msg;
    return false;
  }

  DCHECK(NULL == shader_);
  HRESULT hr = d3d_device->CreatePixelShader(blob->GetBufferPointer(),
                                             blob->GetBufferSize(),
                                             NULL,
                                             &shader_);
  HRESULT_HANDLE(hr, ERROR, "CreateVertexShader failed ");
  return true;
}

bool D3DGeometryGpuProgram::Init(RenderSystem* vrs) {
  D3DRenderSystem* rs = (D3DRenderSystem*)vrs;
  DCHECK(NULL == shader_);
  std::string msg;
  ID3D11Device* d3d_device = rs->GetDevice();
  D3DBlobPtr blob(CompileShaderForStage(stage(), info_.code, info_.path, &msg));
  if (NULL == blob) {
    LOG(ERROR) << "Failed to compiler shader: " << msg;
    return false;
  }

  HRESULT hr = d3d_device->CreateGeometryShader(blob->GetBufferPointer(),
                                                blob->GetBufferSize(),
                                                NULL,
                                                &shader_);
  HRESULT_HANDLE(hr, ERROR, "CreateVertexShader failed ");
  DCHECK(NULL != shader_);
  return true;
}

bool D3DHullGpuProgram::Init(RenderSystem* vrs) {
  D3DRenderSystem* rs = (D3DRenderSystem*)vrs;
  DCHECK(NULL == shader_);
  std::string msg;
  ID3D11Device* d3d_device = rs->GetDevice();
  D3DBlobPtr blob(CompileShaderForStage(stage(), info_.code, info_.path, &msg));
  if (NULL == blob) {
    LOG(ERROR) << "Failed to compiler shader: " << msg;
    return false;
  }

  HRESULT hr = d3d_device->CreateHullShader(blob->GetBufferPointer(),
                                            blob->GetBufferSize(),
                                            NULL,
                                            &shader_);
  HRESULT_HANDLE(hr, ERROR, "CreateVertexShader failed ");
  DCHECK(NULL != shader_);
  return true;
}

bool D3DDomainGpuProgram::Init(RenderSystem* vrs) {
  D3DRenderSystem* rs = (D3DRenderSystem*)vrs;
  DCHECK(NULL == shader_);
  std::string msg;
  ID3D11Device* d3d_device = rs->GetDevice();
  D3DBlobPtr blob(CompileShaderForStage(stage(), info_.code, info_.path, &msg));
  if (NULL == blob) {
    LOG(ERROR) << "Failed to compiler shader: " << msg;
    return false;
  }

  HRESULT hr = d3d_device->CreateDomainShader(blob->GetBufferPointer(),
                                              blob->GetBufferSize(),
                                              NULL,
                                              &shader_);
  HRESULT_HANDLE(hr, ERROR, "CreateVertexShader failed ");
  DCHECK(NULL != shader_);
  return true;
}
}  // namespace d3d11
}  // namespace azer
