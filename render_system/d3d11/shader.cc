#include "azer/render_system/d3d11/shader.h"

#include <d3dcompiler.h>
#include "base/strings/string_util.h"
#include "base/strings/string_split.h"
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
  D3DBlobPtr blob(CompileHLSL(info_, &msg));
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
bool D3DVertexShader::InitResource(ID3D11Device* d3d_device, ID3DBlob* blob) {
  DCHECK(NULL == resource_);
  HRESULT hr = d3d_device->CreateVertexShader(blob->GetBufferPointer(),
                                              blob->GetBufferSize(),
                                              NULL,
                                              &resource_);
  HRESULT_HANDLE(hr, ERROR, "CreateVertexShader failed ");

  // check layout is validate for shader
  scoped_refptr<D3DVertexLayout> layout(new D3DVertexLayout(desc_.get()));
  if (!layout->ValidateShaderLayout(RenderSystem::Current(), blob)) {
    return false;
  }

  return true;
}

// class D3DGeometryShader
D3DGeometryShader::D3DGeometryShader(const ShaderInfo& info) 
    : D3DShader(info), resource_(NULL) {}
D3DGeometryShader::D3DGeometryShader(VertexDesc* desc, const ShaderInfo& info) 
    : D3DShader(desc, info), resource_(NULL) {}
D3DGeometryShader::~D3DGeometryShader() {SAFE_RELEASE(resource_);}

bool D3DGeometryShader::Init(RenderSystem* vrs)  {
  D3DRenderSystem* rs = (D3DRenderSystem*)vrs;
  ID3D11Device* d3ddevice = rs->GetDevice();
  D3DBlobPtr blob;
  blob = CompileShader(d3ddevice);
  if (!blob.get()) {
    return false;
  }

  return InitResource(d3ddevice, blob.get());
}

namespace {
int32_t ComponentCount(DataFormat type) {
  switch (type) {
    case kFloat: return 1;
    case kByteVec2: return 2;
    case kByteVec3: return 3;
    case kByteVec4: return 4;
    case kShortVec2: return 2;
    case kShortVec3: return 3;
    case kShortVec4: return 4;
    case kVec2: return 2;
    case kVec3: return 3;
    case kVec4: return 4;
    case kMat2: return 4;
    case kMat3: return 9;
    case kMat4: return 16;
    case kInt: return 1;
    case kByteIntVec2: return 2;
    case kByteIntVec3: return 3;
    case kByteIntVec4: return 4;
    case kShortIntVec2: return 2;
    case kShortIntVec3: return 3;
    case kShortIntVec4: return 4;
    case kIntVec2: return 2;
    case kIntVec3: return 3;
    case kIntVec4: return 4;
    case kUint: return 1;
    case kByteUintVec2: return 2;
    case kByteUintVec3: return 3;
    case kByteUintVec4: return 4;
    case kShortUintVec2: return 2;
    case kShortUintVec3: return 3;
    case kShortUintVec4: return 4;
    case kUintVec2: return 2;
    case kUintVec3: return 3;
    case kUintVec4: return 4;
    case kBool: return 1;
    case kBoolVec2: return 2;
    case kBoolVec3: return 3;
    case kBoolVec4: return 4;
    default: CHECK(false); return -1;
  }
}
}

bool D3DGeometryShader::InitResource(ID3D11Device* d3ddevice, ID3DBlob* blob) {
  HRESULT hr;
  if (!vertex_desc()) {
    hr = d3ddevice->CreateGeometryShader(blob->GetBufferPointer(),
                                         blob->GetBufferSize(),
                                         NULL,
                                         &resource_);
    HRESULT_HANDLE(hr, ERROR, "CreateGeometryShader failed ");
  } else {
    const int32_t kMaxEntry = 64;
    D3D11_SO_DECLARATION_ENTRY pDecl[kMaxEntry];
    memset(pDecl, 0, sizeof(pDecl));
    DCHECK_LT(vertex_desc()->element_count(), static_cast<int>(sizeof(pDecl)));
    for (int32_t i = 0; i < vertex_desc()->element_count(); ++i) {
      const VertexDesc::Desc* desc = vertex_desc()->descs() + i;
      pDecl[i].SemanticName = desc->name;
      pDecl[i].Stream = 0;
      pDecl[i].SemanticIndex = desc->semantic_index;
      pDecl[i].StartComponent = 0;
      pDecl[i].ComponentCount = ComponentCount(desc->type);
      pDecl[i].OutputSlot = desc->input_slot;
    }

    scoped_refptr<D3DVertexLayout> layout(new D3DVertexLayout(desc_.get()));
    hr = d3ddevice->CreateGeometryShaderWithStreamOutput(
        blob->GetBufferPointer(), blob->GetBufferSize(),
        pDecl, vertex_desc()->element_count(),
        NULL, 0, 0, NULL, &resource_);
    HRESULT_HANDLE(hr, ERROR, "CreateGeometryShader failed ");
  }
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

const uint32_t kCommflags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR 
    | D3DCOMPILE_ENABLE_STRICTNESS;

const uint32_t kDefaultCompileFlags =
    D3DCOMPILE_DEBUG
    | D3DCOMPILE_PREFER_FLOW_CONTROL
    | D3DCOMPILE_SKIP_OPTIMIZATION
    | kCommflags;

const uint32_t CalcFlags(const std::string& options) {
  uint32_t flags = kDefaultCompileFlags;
  std::vector<std::string> vec = base::SplitString(
      options, ",", ::base::TRIM_WHITESPACE, ::base::SPLIT_WANT_NONEMPTY);
  for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
    if (*iter == "release") {
      flags ^= D3DCOMPILE_DEBUG;
    }
    if (*iter == "debug") {
      flags |= D3DCOMPILE_DEBUG;
    }
    if (*iter == "strictness") {
      flags |= D3DCOMPILE_ENABLE_STRICTNESS;
    }
    if (*iter == "prefer_flow_control") {
      flags |= D3DCOMPILE_PREFER_FLOW_CONTROL;
    }
    if (*iter == "optimization") {
      flags ^= D3DCOMPILE_SKIP_OPTIMIZATION;
    }
  }

  return flags;
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
ID3DBlob* CompileHLSL(const ShaderInfo& info, std::string* error_msg) {
  RenderPipelineStage stage = (RenderPipelineStage)info.stage;
  std::string source_path = info.path.empty() ? "noname.hlsl" : info.path;
  const char* entry_name = info.entry.empty() ? 
      DefaultShaderEntryForStage(stage) : info.entry.c_str();
  const char* target = info.version.empty() ? 
      DefaultShaderVersionForStage(stage) : info.version.c_str();
  
  ID3DBlob* blob = NULL;
  ID3DBlob* msgblob = NULL;
  HRESULT hr;
  const uint32_t compile_flags = CalcFlags(info.options);
  hr = D3DCompile(info.code.c_str(),
                  info.code.length(),
                  source_path.c_str(),
                  NULL,
                  D3D_COMPILE_STANDARD_FILE_INCLUDE, // pIncludes
                  entry_name, // pEntryPointer
                  target,
                  compile_flags,
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
