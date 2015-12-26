#include "azer/render_system/d3d11/hlsl_compile.h"

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dcompiler.h>
#include <xnamath.h>

namespace azer {
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
    default: 
      CHECK(false) << " not support :" << format;
      return "";
  }
}
}  // namespace azer
