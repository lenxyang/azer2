#include "azer/render_system/d3d11/util.h"

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

ID3DBlob* CompileHLSL(const std::string& shader, const std::string& target,
                      const std::string& entrypointer,
                      const std::string& shader_path,
                      std::string* error_msg) {
  ID3DBlob* blob = NULL;
  ID3DBlob* msgblob = NULL;
  HRESULT hr;
  hr = D3DCompile(shader.c_str(),
                  shader.length(),
                  shader_path.c_str(),
                  NULL,
                  NULL,                 // pIncludes
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
  return CompileHLSL(shader, "gs_5_0", entry_name, path, error_msg);
}

const char* DefaultShaderEntryForStage(RenderPipelineStage stage) {
  switch (stage) {
    case kVertexStage: return "vs_main";
    case kConstantsHullStage: return "consts_hull_main";
    case kControlPointHullStage: return "hull_main";
    case kDomainStage: return "domain_main";
    case kGeometryStage: return "gs_main";
    case kPixelStage: return "ps_main";
    default: CHECK(false); return "";
  }
}

}  // namespace azer
