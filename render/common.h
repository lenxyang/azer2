#pragma once

#include <stdint.h>
#include <ostream>
#include "azer/base/export.h"

namespace azer {

enum CullingMode {
  kCullNone = 1,     // none
  kCullFront,        //
  kCullBack,         // counter clock-wise
};

enum FillMode {
  kWireFrame = 2,
  kSolid,
};

enum FrontFace {
  kCounterClockwise = 1,
  kClockwise,
};

enum CPUAccess {
  kCPUNoAccess   = 0x0,
  kCPUWrite      = 0x10000L,
  kCPURead       = 0x20000L,
  kCPUReadWrite  = kCPUWrite | kCPURead,
};

// n:  normal
// us: unsigned
// s:  signed
// f:  float
// tl: typeless
enum DataFormat {
  kScalar = 1,
  kFloat = kScalar,
  kByteVec2,
  kByteVec3,
  kByteVec4,
  kShortVec2,
  kShortVec3,
  kShortVec4,
  kVec2,
  kVec3,
  kVec4,
  kMat2,
  kMat3,
  kMat4,
  kByteInt,
  kShortInt,
  kInt,
  kByteIntVec2,
  kByteIntVec3,
  kByteIntVec4,
  kShortIntVec2,
  kShortIntVec3,
  kShortIntVec4,
  kIntVec2,
  kIntVec3,
  kIntVec4,
  kUint,
  kByteUintVec2,
  kByteUintVec3,
  kByteUintVec4,
  kShortUintVec2,
  kShortUintVec3,
  kShortUintVec4,
  kUintVec2,
  kUintVec3,
  kUintVec4,
  kBool,
  kBoolVec2,
  kBoolVec3,
  kBoolVec4,
};

enum PrimitiveTopology {
  kPointList = 1,
  kLineStrip,
  kLineList,
  kTriangleStrip,
  kTriangleList,
  kControlPoint1,
  kControlPoint2,
  kControlPoint3,
  kControlPoint4,
  kControlPoint5,
  kControlPoint6,
  kControlPoint7,
  kControlPoint8,
  kControlPoint9,
  kControlPoint10,
  kControlPoint11,
  kControlPoint12,
  kControlPoint13,
  kControlPoint14,
  kControlPoint15,
  kControlPoint16,
  kControlPoint17,
  kControlPoint18,
  kControlPoint19,
  kControlPoint20,
  kControlPoint21,
  kControlPoint22,
  kControlPoint23,
  kControlPoint24,
  kControlPoint25,
  kControlPoint26,
  kControlPoint27,
  kControlPoint28,
  kControlPoint29,
  kControlPoint30,
  kControlPoint31,
  kControlPoint32,
};

enum RenderPipelineStage {
  kStageNotSpec = 0,
  kInputAssemblerStage = 1,
  kVertexStage,
  kHullStage,
  kTessellatorStage,
  kDomainStage,
  kComputeStage,
  kGeometryStage,
  kStreamOutStage,
  kRasterizerStage,
  kPixelStage,
  kOutputMergerStage,
  kRenderPipelineStageNum,
};

/**
 *
 */
enum BufferUsage {
  kBufferDefault = 1,  //  require read and write acceed by th GPU
  kBufferGPUReayOnly,  // can only read by GPU, 
  kBufferDynamic,      // GPU(read only), CPU(write only)
  kBufferStaging,      // supports transform resource from GPU to CPU
};

enum class CompareFunc {
  kNever = 1,
  kLess,
  kEqual,
  kLessEqual,
  kGreater,
  kNotEqueal,
  kGreaterEqual,
  kAlways,
};

AZER_EXPORT std::ostream& operator << (std::ostream& os, const CompareFunc& func);

enum class StencilOper {
  kKeep,
  kZero,
  kReplace,
  kIncrSat,
  kDecrSat,
  kInvert,
  kIncr,
  kDecr,
};

enum class MapType {
  kReadOnly,
  kWrite,
  kReadWrite,
  kWriteDiscard,
  kWriteNoOverwrite,
};

enum VisibleState {
  kNoneVisible    = -1,
  kFullyVisible   = 0,
  kPartialVisible = 1,
};

enum class TexType {
  kUnknown = 0,
  k1D = 1,
  k1DArray,
  k2D,
  k2DArray,
  k2DMultiSample,
  k2DArrayMultiSample,
  k3D,
  kCubemap,
  kCubemapArray,
};

AZER_EXPORT std::ostream& operator << (std::ostream& os, const TexType& t);

enum class TexAddressMode {
  kWrap,
  kMirror,
  kClamp,
  kBorder,
  kMirrorOnce,
};

enum class FilterMode {
  kPoint = 1,
  kLinear,
};

enum BindTarget {
  kBindTargetUnknown           = 0,
  kBindTargetShaderResource    = 0x0008L,
  kBindTargetRenderTarget      = 0x0020L,
  kBindTargetDepthStencil      = 0x0040L,
  kBindTargetUnorderedAccess   = 0x0080L,
  kBindTargetVertexBuffer      = 0x0100L,
  kBindTargetIndicesBuffer     = 0x0200L,
  kBindTargetStreamOut         = 0x0400L,
  kBindTargetContantBuffer     = 0x0800L,
};


AZER_EXPORT const char* RenderStageName(RenderPipelineStage stage);
AZER_EXPORT const char* GetDataFormatName(DataFormat format);
AZER_EXPORT uint32_t SizeofDataFormat(DataFormat format);
}  // namespace azer
