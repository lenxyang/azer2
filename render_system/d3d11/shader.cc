#include "azer/render_system/d3d11/shader.h"

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dcompiler.h>
#include <xnamath.h>

#include "azer/base/string.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/vertex_buffer.h"

namespace azer {
namespace d3d11 {
// clsas D3DShader
D3DShader::D3DShader(const ShaderInfo& info) : Shader(info) {}
D3DShader::D3DShader(VertexDesc* desc, const ShaderInfo& info) 
    : Shader(desc, info) {}
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

// class D3DVertexShader
D3DVertexShader::D3DVertexShader(VertexDesc* desc, const ShaderInfo& info)
    : D3DShader(desc, info)
    , resource_(NULL) {
}

D3DVertexShader::~D3DVertexShader() { SAFE_RELEASE(resource_);}
bool D3DVertexShader::Init(RenderSystem* vrs) {
  D3DRenderSystem* rs = (D3DRenderSystem*)vrs;
  std::string msg;
  ID3D11Device* d3d_device = rs->GetDevice();

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
  scoped_refptr<D3DVertexLayout> layout(new D3DVertexLayout(desc_));
  if (!layout->ValidateShaderLayout(vrs, blob.get())) {
    return false;
  }

  return true;
}

// class D3DGeometryShader
D3DGeometryShader::D3DGeometryShader(const ShaderInfo& info) 
    : D3DShader(info), resource_(NULL) {}
D3DGeometryShader::~D3DGeometryShader() {SAFE_RELEASE(resource_);}

bool D3DGeometryShader::Init(RenderSystem* vrs)  {
  D3DRenderSystem* rs = (D3DRenderSystem*)vrs;
  ID3D11Device* d3ddevice = rs->GetDevice();
  D3DBlobPtr blob;
  if (!vertex_desc()) {
    blob = CompileShader(d3ddevice);
    if (!blob.get()) {
      return false;
    }
  } else {
    
  }

  return InitResource(d3ddevice, blob.get());
}
bool D3DGeometryShader::InitResource(ID3D11Device* d3ddevice, ID3DBlob* blob) {
  HRESULT hr = d3ddevice->CreateGeometryShader(blob->GetBufferPointer(),
                                               blob->GetBufferSize(),
                                               NULL,
                                               &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreateGeometryShader failed ");
  return true;
}

// class D3DHullShader
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

// class D3DDomainShader
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

// class D3DPixelShader
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

// class D3DComputeShader
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

#if defined(_DEBUG)
const DWORD kCompileFlags =
    D3DCOMPILE_DEBUG
    | D3DCOMPILE_ENABLE_STRICTNESS
    | D3DCOMPILE_OPTIMIZATION_LEVEL0
    | D3DCOMPILE_PREFER_FLOW_CONTROL
    | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
const DWORD kCompileFlags =
    D3DCOMPILE_ENABLE_STRICTNESS
    | D3DCOMPILE_SKIP_VALIDATION
    ;
#endif

namespace {
const char* DefaultShaderVersionForStage(RenderPipelineStage stage);
}

ID3DBlob* CompileHLSL(const std::string& shader, const std::string& target,
                      const std::string& entrypointer,
                      const std::string& shader_path,
                      std::string* error_msg) {
  std::string source_path = shader_path.empty() ? "noname.hlsl" : shader_path;
  ID3DBlob* blob = NULL;
  ID3DBlob* msgblob = NULL;
  HRESULT hr;
  hr = D3DCompile(shader.c_str(),
                  shader.length(),
                  source_path.c_str(),
                  NULL,
                  D3D_COMPILE_STANDARD_FILE_INCLUDE, // pIncludes
                  entrypointer.c_str(), // pEntryPointer
                  target.c_str(),
                  kCompileFlags,
                  0,
                  &blob,
                  &msgblob);

  if (FAILED(hr)) {
    error_msg->append((const char*)msgblob->GetBufferPointer(),
                      msgblob->GetBufferSize());
    return NULL;
  }
  return blob;
}

ID3DBlob* CompileShaderForStage(RenderPipelineStage stage, 
                                const std::string& shader, 
                                const std::string& path, 
                                std::string* error_msg) {
  const char* entry_name = DefaultShaderEntryForStage(stage);
  const char* version_name = DefaultShaderVersionForStage(stage);
  return CompileHLSL(shader, version_name, entry_name, path, error_msg);
}

const char* DefaultShaderEntryForStage(RenderPipelineStage stage) {
  switch (stage) {
    case kVertexStage: return "vs_main";
    case kHullStage: return "hs_main";
    case kDomainStage: return "ds_main";
    case kGeometryStage: return "gs_main";
    case kPixelStage: return "ps_main";
    case kComputeStage: return "cs_main";
    default: CHECK(false); return "";
  }
}

namespace {
const char* DefaultShaderVersionForStage(RenderPipelineStage stage) {
  switch (stage) {
    case kVertexStage: return "vs_5_0";
    case kHullStage: return "hs_5_0";
    case kDomainStage: return "ds_5_0";
    case kGeometryStage: return "gs_5_0";
    case kPixelStage: return "ps_5_0";
    case kComputeStage: return "cs_5_0";
    default: CHECK(false); return "";
  }
}
}  // namespace

const char* HLSLTypeName(DataFormat format) {
  switch (format) {
    case kFloat: return "float";
    case kVec2: return "float2";
    case kVec3: return "float3";
    case kVec4: return "float4";
    case kMat2: return "float2x2";
    case kMat3: return "float3x3";
    case kMat4: return "float4x4";
    case kInt: return "int";
    case kIntVec2: return "int2";
    case kIntVec3: return "int3";
    case kIntVec4: return "int4";
    case kUint: return "uint";
    case kUintVec2: return "uint2";
    case kUintVec3: return "uint3";
    case kUintVec4: return "uint4";
    default: 
      CHECK(false) << " not support :" << format;
      return "";
  }
}
}  // namespace d3d11
}  // namespace azer
