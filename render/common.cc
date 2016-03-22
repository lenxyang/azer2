#include "azer/render/common.h" 

#include "base/logging.h"
#include "base/basictypes.h"

namespace azer {
const char* RenderStageName(RenderPipelineStage stage) {
  switch (stage) {
    case kInputAssemblerStage: return "InputAssemblerStage";
    case kVertexStage: return "VertexStage";
    case kHullStage: return "HullStage";
    case kTessellatorStage: return "TessellatorStage";
    case kDomainStage: return "DomainStage";
    case kGeometryStage: return "GeometryStage";
    case kStreamOutStage: return "StreamOutStage";
    case kRasterizerStage: return "RasterizerStage";
    case kPixelStage: return "PixelStage";
    case kOutputMergerStage: return "OutputMergerStage";
    case kComputeStage: return "ComputeStage";
    case kRenderPipelineStageNum: return "RenderPipelineStageNum";
    default: NOTREACHED(); return "";
  }
}

const char* GetDataFormatName(DataFormat format) {
  switch (format) {
    case kFloat: return "float";
    case kVec2: return "vec2";
    case kVec3: return "vec3";
    case kVec4: return "vec4";
    case kMat2: return "mat2";
    case kMat3: return "mat3";
    case kMat4: return "mat4";
    case kInt: return "int";
    case kIntVec2: return "intvec2";
    case kIntVec3: return "intvec3";
    case kIntVec4: return "intvec4";
    case kUint: return "uint";
    case kUintVec2: return "uintvec2";
    case kUintVec3: return "uintvec3";
    case kUintVec4: return "uintvec4";
    case kBool: return "bool";
    case kBoolVec2: return "boolvec2";
    case kBoolVec3: return "boolvec3";
    case kBoolVec4: return "boolvec4";
    case kRGBA8: return "rgba(8)";
    case kRGBAn8: return "rgba(norm8)";
    case kRGBA32: return "rgba(32)";
    case kRGBAn32:return "rgba(norm32)";
    case kRGBAf: return "rgba(f)";
    case kR11G11B10f: return "r11g11b10";
    case kDepth24nStencil8u: return "Depth24nStencil8u";
    case kR24G8: return "kR24G8";
    case kR24nX8: return "kR24nX8";
    case kDXBC1n: return "DXBC1n";
    case kDXBC1nSRGB: return "DXBC1nSRGB";
    case kDXBC4s: return "DXBC4s";
    case kDXBC4n: return "DXBC4n";
    case kDXBC2n: return "DXBC2n";
    case kDXBC2nSRGB: return "DXBC2nSRGB";
    case kDXBC3n: return "DXBC3n";
    case kDXBC3nSRGB: return "DXBC3nSRGB";
    case kDXBC5s: return "DXBC5s";
    case kDXBC5n: return "DXBC5n";
    case kDXBC7n: return "DXBC7n";
    case kDXBC7nSRGB: return "DXBC7nSRGB";
      // case kRGBA: return sizeof(float) * 4;
    default:
      NOTREACHED();
      return "";
  }
}


uint32 SizeofDataFormat(DataFormat format) {
  switch (format) {
    case kFloat: return sizeof(float);
    case kVec2: return sizeof(float) * 2;
    case kVec3: return sizeof(float) * 3;
    case kVec4: return sizeof(float) * 4;
    case kMat2: return sizeof(float) * 4;
    case kMat3: return sizeof(float) * 9;
    case kMat4: return sizeof(float) * 16;
    case kInt: return sizeof(int32);
    case kIntVec2: return sizeof(int32) * 2;
    case kIntVec3: return sizeof(int32) * 3;
    case kIntVec4: return sizeof(int32) * 4;
    case kUint: return sizeof(uint32);
    case kUintVec2: return sizeof(uint32) * 2;
    case kUintVec3: return sizeof(uint32) * 3;
    case kUintVec4: return sizeof(uint32) * 4;
    case kBool: return sizeof(int32);
    case kBoolVec2: return sizeof(int32) * 2;
    case kBoolVec3: return sizeof(int32) * 3;
    case kBoolVec4: return sizeof(int32) * 4;
    case kRGBA8: return sizeof(int8) * 4;
    case kRGBAn8: return sizeof(uint8) * 4;
    case kRGBA32: return sizeof(int32) * 4;
    case kRGBAn32: return sizeof(uint32) * 4;
    case kRGBAf: return sizeof(float) * 4;
    case kDepth24nStencil8u:
    case kR24G8:
    case kR24nX8:
      return sizeof(uint32);
      // case kRGBA: return sizeof(float) * 4;
    case kDXBC1n:
    case kDXBC1nSRGB:
    case kDXBC4n:
    case kDXBC4s:
      return 4;
    case kDXBC2n:
    case kDXBC2nSRGB:
    case kDXBC3n:
    case kDXBC3nSRGB:
    case kDXBC5s:
    case kDXBC5n:
    case kDXBC7n:
    case kDXBC7nSRGB:
      return 8;
    default:
      NOTREACHED();
      return 0;
  }
};
}  // namespace azer
