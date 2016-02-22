#pragma once

#include "azer/base/export.h"
#include "base/basictypes.h"

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
  kCPUNoAccess = 0x0,
  kCPUWrite    = 0x10000L,
  kCPURead     = 0x20000L,
  kCPUAccess   = kCPUWrite | kCPURead,
};

enum DataFormat {
  kScalar = 1,
  kFloat = kScalar,
  kVec2,
  kVec3,
  kVec4,
  kMat2,
  kMat3,
  kMat4,
  kInt,
  kIntVec2,
  kIntVec3,
  kIntVec4,
  kUint,
  kUintVec2,
  kUintVec3,
  kUintVec4,
  kBool,
  kBoolVec2,
  kBoolVec3,
  kBoolVec4,
  kRGBA8,             // all element are uint8
  kRGBAn8,            // all element are uint8
  kRGBA32,            // all element are uint32
  kRGBAn32,           // all element are uint32
  kRGBAf,             // all element are float
  kBGRA8,
  kBGRAn8,
  kBGRA32,
  kBGRAn32,
  kBGRAnf,
  kRGBA = kRGBAf,
  kDepth24Stencil8,
  kDXBC1n,
  kDXBC1nSRGB,
  kDXBC4n,
  kDXBC4nSRGB,
  kDXBC2n,
  kDXBC2nSRGB,
  kDXBC3n,
  kDXBC3nSRGB,
  kDXBC5n,
  kDXBC5nSRGB,
  kDXBC7n,
  kDXBC7nSRGB,
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
  kStageNotSpec = -1,
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

enum CompareFunc {
  kCompareFuncNever = 1,
  kCompareFuncLess,
  kCompareFuncEqual,
  kCompareFuncLessEqual,
  kCompareFuncGreater,
  kCompareFuncNotEqueal,
  kCompareFuncGreaterEqua,
  kCompareFuncAlways,
};

enum StencilOper {
  kStencilOperKeep,
  kStencilOperZero,
  kStencilOperReplace,
  kStencilOperIncrSat,
  kStencilOperDecrSat,
  kStencilOperInvert,
  kStencilOperIncr,
  kStencilOperDecr,
};

enum MapType {
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

enum TexType {
  kUnkonwnTexType = -1,
  kTex1D = 1,
  kTex1DArray,
  kTex2D,
  kTex2DArray,
  kTex2DMultiSample,
  kTex2DArrayMultiSample,
  kTex3D,
  kTexCubemap,
  kTexCubemapArray,
};

enum AddressMode {
  kTexAddrModeWrap,
  kTexAddrModeMirror,
  kTexAddrModeClamp,
  kTexAddrModeBorder,
  kTexAddrModeMirrorOnce,
};

enum FilterMode {
  kFilterModePoint = 1,
  kFilterModeLinear,
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
AZER_EXPORT uint32 SizeofDataFormat(DataFormat format);
}  // namespace azer
