#include "azer/render/common.h" 

#include "base/logging.h"


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


uint32_t SizeofDataFormat(DataFormat format) {
  switch (format) {
    case kFloat: return sizeof(float);
    case kByteVec2: return sizeof(uint8_t) * 2;
    case kByteVec3: return sizeof(uint8_t) * 3;
    case kByteVec4: return sizeof(uint8_t) * 4;
    case kShortVec2: return sizeof(uint16_t) * 2;
    case kShortVec3: return sizeof(uint16_t) * 3;
    case kShortVec4: return sizeof(uint16_t) * 4;
    case kVec2: return sizeof(float) * 2;
    case kVec3: return sizeof(float) * 3;
    case kVec4: return sizeof(float) * 4;
    case kMat2: return sizeof(float) * 4;
    case kMat3: return sizeof(float) * 9;
    case kMat4: return sizeof(float) * 16;
    case kInt: return sizeof(int32_t);
    case kByteIntVec2: return sizeof(uint8_t) * 2;
    case kByteIntVec3: return sizeof(uint8_t) * 3;
    case kByteIntVec4: return sizeof(uint8_t) * 4;
    case kShortIntVec2: return sizeof(uint16_t) * 2;
    case kShortIntVec3: return sizeof(uint16_t) * 3;
    case kShortIntVec4: return sizeof(uint16_t) * 4;
    case kIntVec2: return sizeof(int32_t) * 2;
    case kIntVec3: return sizeof(int32_t) * 3;
    case kIntVec4: return sizeof(int32_t) * 4;
    case kUint: return sizeof(uint32_t);
    case kByteUintVec2: return sizeof(uint8_t) * 2;
    case kByteUintVec3: return sizeof(uint8_t) * 3;
    case kByteUintVec4: return sizeof(uint8_t) * 4;
    case kShortUintVec2: return sizeof(uint16_t) * 2;
    case kShortUintVec3: return sizeof(uint16_t) * 3;
    case kShortUintVec4: return sizeof(uint16_t) * 4;
    case kUintVec2: return sizeof(uint32_t) * 2;
    case kUintVec3: return sizeof(uint32_t) * 3;
    case kUintVec4: return sizeof(uint32_t) * 4;
    case kBool: return sizeof(int32_t);
    case kBoolVec2: return sizeof(int32_t) * 2;
    case kBoolVec3: return sizeof(int32_t) * 3;
    case kBoolVec4: return sizeof(int32_t) * 4;
    default:
      NOTREACHED();
      return 0;
  }
};



std::ostream& operator << (std::ostream& os, const CompareFunc& func) {
  switch (func) {
    case CompareFunc::kNever: os << "CompareFunc::kNever"; break;
    case CompareFunc::kLess: os << "CompareFunc::kLess"; break;
    case CompareFunc::kEqual: os << "CompareFunc::kEqual"; break;
    case CompareFunc::kLessEqual: os << "CompareFunc::kLessEqual"; break;
    case CompareFunc::kGreater: os << "CompareFunc::kGreater"; break;
    case CompareFunc::kNotEqueal: os << "CompareFunc::kNotEqueal"; break;
    case CompareFunc::kGreaterEqual: os << "CompareFunc::kGreaterEqual"; break;
    case CompareFunc::kAlways: os << "CompareFunc::kAlways"; break;
    default: CHECK(false) << "Unknonw Comparefunc";
  }
  return os;
}

std::ostream& operator << (std::ostream& os, const TexType& t) {
  switch (t) {
    case TexType::kUnknown: os << "TexType::kUnknown"; break;
    case TexType::k1D: os << "TexType::k1D"; break;
    case TexType::k1DArray: os << "TexType::k1DArray"; break;
    case TexType::k2D: os << "TexType::k2D"; break;
    case TexType::k2DArray: os << "TexType::k2DArray"; break;
    case TexType::k2DMultiSample: os << "TexType::k2DMultiSample"; break;
    case TexType::k2DArrayMultiSample: os << "TexType::k2DArrayMultiSample"; break;
    case TexType::k3D: os << "TexType::k3D"; break;
    case TexType::kCubemap: os << "TexType::kCubemap"; break;
    case TexType::kCubemapArray: os << "TexType::kCubemapArray"; break;
    default: CHECK(false) << "Unknown TexType";
  }
  return os;
}
}  // namespace azer
