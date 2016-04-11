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
    case kByteVec2: return "bytevec2";
    case kByteVec3: return "bytevec3";
    case kByteVec4: return "bytevec4";
    case kShortVec2: return "shortvec2";
    case kShortVec3: return "shortvec3";
    case kShortVec4: return "shortvec4";
    case kVec2: return "vec2";
    case kVec3: return "vec3";
    case kVec4: return "vec4";
    case kMat2: return "mat2";
    case kMat3: return "mat3";
    case kMat4: return "mat4";
    case kInt: return "int";
    case kByteIntVec2: return "byteint2";
    case kByteIntVec3: return "byteint3";
    case kByteIntVec4: return "byteint4";
    case kShortIntVec2: return "shortint2";
    case kShortIntVec3: return "shortint3";
    case kShortIntVec4: return "shortint4";
    case kIntVec2: return "intvec2";
    case kIntVec3: return "intvec3";
    case kIntVec4: return "intvec4";
    case kUint: return "uint";
    case kByteUintVec2: return "byteuint2";
    case kByteUintVec3: return "byteuint3";
    case kByteUintVec4: return "byteuint4";
    case kShortUintVec2: return "shortuint2";
    case kShortUintVec3: return "shortuint3";
    case kShortUintVec4: return "shortuint4";
    case kUintVec2: return "uintvec2";
    case kUintVec3: return "uintvec3";
    case kUintVec4: return "uintvec4";
    case kBool: return "bool";
    case kBoolVec2: return "boolvec2";
    case kBoolVec3: return "boolvec3";
    case kBoolVec4: return "boolvec4";
    default:
      NOTREACHED();
      return "";
  }
}


uint32 SizeofDataFormat(DataFormat format) {
  switch (format) {
    case kFloat: return sizeof(float);
    case kByteVec2: return sizeof(uint8) * 2;
    case kByteVec3: return sizeof(uint8) * 3;
    case kByteVec4: return sizeof(uint8) * 4;
    case kShortVec2: return sizeof(uint16) * 2;
    case kShortVec3: return sizeof(uint16) * 3;
    case kShortVec4: return sizeof(uint16) * 4;
    case kVec2: return sizeof(float) * 2;
    case kVec3: return sizeof(float) * 3;
    case kVec4: return sizeof(float) * 4;
    case kMat2: return sizeof(float) * 4;
    case kMat3: return sizeof(float) * 9;
    case kMat4: return sizeof(float) * 16;
    case kInt: return sizeof(int32);
    case kByteIntVec2: return sizeof(uint8) * 2;
    case kByteIntVec3: return sizeof(uint8) * 3;
    case kByteIntVec4: return sizeof(uint8) * 4;
    case kShortIntVec2: return sizeof(uint16) * 2;
    case kShortIntVec3: return sizeof(uint16) * 3;
    case kShortIntVec4: return sizeof(uint16) * 4;
    case kIntVec2: return sizeof(int32) * 2;
    case kIntVec3: return sizeof(int32) * 3;
    case kIntVec4: return sizeof(int32) * 4;
    case kUint: return sizeof(uint32);
    case kByteUintVec2: return sizeof(uint8) * 2;
    case kByteUintVec3: return sizeof(uint8) * 3;
    case kByteUintVec4: return sizeof(uint8) * 4;
    case kShortUintVec2: return sizeof(uint16) * 2;
    case kShortUintVec3: return sizeof(uint16) * 3;
    case kShortUintVec4: return sizeof(uint16) * 4;
    case kUintVec2: return sizeof(uint32) * 2;
    case kUintVec3: return sizeof(uint32) * 3;
    case kUintVec4: return sizeof(uint32) * 4;
    case kBool: return sizeof(int32);
    case kBoolVec2: return sizeof(int32) * 2;
    case kBoolVec3: return sizeof(int32) * 3;
    case kBoolVec4: return sizeof(int32) * 4;
    default:
      NOTREACHED();
      return 0;
  }
};
}  // namespace azer
