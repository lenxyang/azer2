#include "azer/render_system/d3d11/enum_transform.h"

#include "base/logging.h"

namespace azer {
namespace d3d11 {

D3D11_USAGE TranslateUsage(BufferUsage usage) {
  switch(usage) {
    case kBufferDefault: return  D3D11_USAGE_DEFAULT;
    case kBufferGPUReayOnly: return D3D11_USAGE_IMMUTABLE;
    case kBufferDynamic: return D3D11_USAGE_DYNAMIC;
    case kBufferStaging: return D3D11_USAGE_STAGING;
    default:
      DCHECK(false) << "Unsupport usage: " << (int32_t)usage;
      return D3D11_USAGE_DEFAULT;
  }
}

D3D11_CPU_ACCESS_FLAG TranslateCPUAccess(CPUAccess access) {
  switch (access) {
    case kCPUNoAccess: return (D3D11_CPU_ACCESS_FLAG)0x0;
    case kCPUWrite: return D3D11_CPU_ACCESS_WRITE;
    case kCPURead: return D3D11_CPU_ACCESS_READ;
    case kCPUReadWrite:
      return (D3D11_CPU_ACCESS_FLAG)(D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ);
    default:
      DCHECK(false) << "Unsupport CPUAccess: " << (int32_t)access;
      return (D3D11_CPU_ACCESS_FLAG)(D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ);
  }
}

DXGI_FORMAT TranslateFormat(DataFormat type) {
  DXGI_FORMAT none = (DXGI_FORMAT)0;
  switch (type) {
    case kScalar: return DXGI_FORMAT_R32_FLOAT;
    case kByteVec2: return DXGI_FORMAT_R8G8_TYPELESS;
    case kByteVec3: CHECK(false); return none; // no DXGI_FORMAT_R8G8B8_TYPELESS;
    case kByteVec4: return DXGI_FORMAT_R8G8B8A8_TYPELESS;
    case kShortVec2: return DXGI_FORMAT_R16G16_TYPELESS;
    case kShortVec3: CHECK(false); return none; // no DXGI_FORMAT_R16G16B16_TYPELESS;
    case kShortVec4: return DXGI_FORMAT_R16G16B16A16_TYPELESS;
    case kVec2: return DXGI_FORMAT_R32G32_FLOAT;
    case kVec3: return DXGI_FORMAT_R32G32B32_FLOAT;
    case kVec4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case kInt: return DXGI_FORMAT_R32_SINT;
    case kByteIntVec2: return DXGI_FORMAT_R8G8_SINT;
    case kByteIntVec3: CHECK(false); return none; // no DXGI_FORMAT_R8G8B8_SINT;
    case kByteIntVec4: return DXGI_FORMAT_R8G8B8A8_SINT;
    case kShortIntVec2: return DXGI_FORMAT_R16G16_SINT;
    case kShortIntVec3: CHECK(false); return none; // no DXGI_FORMAT_R16G16B16_SINT;
    case kShortIntVec4: return DXGI_FORMAT_R16G16B16A16_SINT;
    case kIntVec2: return DXGI_FORMAT_R32G32_SINT;
    case kIntVec3: return DXGI_FORMAT_R32G32B32_SINT;
    case kIntVec4: return DXGI_FORMAT_R32G32B32A32_SINT;
    case kUint: return DXGI_FORMAT_R32_UINT;
    case kByteUintVec2: return DXGI_FORMAT_R8G8_UINT;
    case kByteUintVec3: CHECK(false); return none; // return DXGI_FORMAT_R8G8B8_UINT;
    case kByteUintVec4: return DXGI_FORMAT_R8G8B8A8_UINT;
    case kShortUintVec2: return DXGI_FORMAT_R16G16_UINT;
    case kShortUintVec3: CHECK(false); return none; // no DXGI_FORMAT_R16G16B16_UINT;
    case kShortUintVec4: return DXGI_FORMAT_R16G16B16A16_UINT;
    case kUintVec2: return DXGI_FORMAT_R32G32_UINT;
    case kUintVec3: return DXGI_FORMAT_R32G32B32_UINT;
    case kUintVec4: return DXGI_FORMAT_R32G32B32A32_UINT;
    case kBool: return DXGI_FORMAT_R32_SINT;
    case kBoolVec2: return DXGI_FORMAT_R32G32_SINT;
    case kBoolVec3: return DXGI_FORMAT_R32G32B32_SINT;
    case kBoolVec4: return DXGI_FORMAT_R32G32B32A32_SINT;
    default:
      DCHECK(false) << "Unsupport type: " << (int32_t)type;
      return DXGI_FORMAT_UNKNOWN;
  }
}


DXGI_FORMAT TranslateTexFormat(TexFormat type) {
  switch (type) {
    case TexFormat::kBGRA8UNorm:     return DXGI_FORMAT_B8G8R8A8_UNORM;
    case TexFormat::kRGBA8UNorm:     return DXGI_FORMAT_R8G8B8A8_UNORM;
    case TexFormat::kRGBA8Uint:      return DXGI_FORMAT_R8G8B8A8_UINT;
    case TexFormat::kR32Sint:        return DXGI_FORMAT_R32_SINT;
    case TexFormat::kR32Uint:        return DXGI_FORMAT_R32_UINT;
    case TexFormat::kR32Float:       return DXGI_FORMAT_R32_FLOAT;
    case TexFormat::kR32Typeless:    return DXGI_FORMAT_R32_TYPELESS;
    case TexFormat::kRG32Sint:       return DXGI_FORMAT_R32G32_SINT;
    case TexFormat::kRG32Uint:       return DXGI_FORMAT_R32G32_UINT;
    case TexFormat::kRG32Float:      return DXGI_FORMAT_R32G32_FLOAT;
    case TexFormat::kRG32Typeless:   return DXGI_FORMAT_R32G32_TYPELESS;
    case TexFormat::kRGB32Sint:      return DXGI_FORMAT_R32G32B32_SINT;
    case TexFormat::kRGB32Uint:      return DXGI_FORMAT_R32G32B32_UINT;
    case TexFormat::kRGB32Float:     return DXGI_FORMAT_R32G32B32_FLOAT;
    case TexFormat::kRGB32Typeless:  return DXGI_FORMAT_R32G32B32_TYPELESS;
    case TexFormat::kRGBA32Sint:     return DXGI_FORMAT_R32G32B32A32_SINT;
    case TexFormat::kRGBA32Uint:     return DXGI_FORMAT_R32G32B32A32_UINT;
    case TexFormat::kRGBA32Float:    return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case TexFormat::kRGBA32Typeless: return DXGI_FORMAT_R32G32B32A32_TYPELESS;
    case TexFormat::kR11G11B10f:     return DXGI_FORMAT_R11G11B10_FLOAT;
    case TexFormat::kD24UNormS8Uint: return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case TexFormat::kR24UNormG8Uint:     return DXGI_FORMAT_R24G8_TYPELESS;
    case TexFormat::kR24UNormX8Typeless: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    case TexFormat::kD32Float:       return DXGI_FORMAT_D32_FLOAT;
    case TexFormat::kDXBC1n:         return DXGI_FORMAT_BC1_UNORM;
    case TexFormat::kDXBC1nSRGB:     return DXGI_FORMAT_BC1_UNORM_SRGB;
    case TexFormat::kDXBC4s:         return DXGI_FORMAT_BC4_SNORM;
    case TexFormat::kDXBC4n:         return DXGI_FORMAT_BC4_UNORM;
    case TexFormat::kDXBC2n:         return DXGI_FORMAT_BC2_UNORM;
    case TexFormat::kDXBC2nSRGB:     return DXGI_FORMAT_BC2_UNORM_SRGB;
    case TexFormat::kDXBC3n:         return DXGI_FORMAT_BC3_UNORM;
    case TexFormat::kDXBC3nSRGB:     return DXGI_FORMAT_BC3_UNORM_SRGB;
    case TexFormat::kDXBC5n:         return DXGI_FORMAT_BC5_UNORM;
    case TexFormat::kDXBC5s:         return DXGI_FORMAT_BC5_SNORM;
    case TexFormat::kDXBC7n:         return DXGI_FORMAT_BC7_UNORM;
    case TexFormat::kDXBC7nSRGB:     return DXGI_FORMAT_BC7_UNORM_SRGB;
    default:
      DCHECK(false) << "Unsupport type: " << (int32_t)type;
      return DXGI_FORMAT_UNKNOWN;
  }
}

D3D_PRIMITIVE_TOPOLOGY TranslatePrimitiveTopology(
    PrimitiveTopology topology) {
  switch(topology) {
    case kPointList: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
    case kLineStrip: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
    case kLineList: return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
    case kTriangleStrip: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    case kTriangleList: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    case kControlPoint1: return D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
    case kControlPoint2: return D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
    case kControlPoint3: return D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
    case kControlPoint4: return D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
    case kControlPoint5: return D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
    case kControlPoint6: return D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
    case kControlPoint7: return D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
    case kControlPoint8: return D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
    case kControlPoint9: return D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
    case kControlPoint10: return D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
    case kControlPoint11: return D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
    case kControlPoint12: return D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
    case kControlPoint13: return D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
    case kControlPoint14: return D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
    case kControlPoint15: return D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
    case kControlPoint16: return D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
    case kControlPoint17: return D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
    case kControlPoint18: return D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
    case kControlPoint19: return D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
    case kControlPoint20: return D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
    case kControlPoint21: return D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
    case kControlPoint22: return D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
    case kControlPoint23: return D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
    case kControlPoint24: return D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
    case kControlPoint25: return D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
    case kControlPoint26: return D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
    case kControlPoint27: return D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
    case kControlPoint28: return D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
    case kControlPoint29: return D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
    case kControlPoint30: return D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
    case kControlPoint31: return D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
    case kControlPoint32: return D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
    default:
      DCHECK(false) << "Unsupport type: " << (int32_t)topology;
      return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
  }
}

DXGI_FORMAT TranslateIndexType(IndexType type) {
  switch(type) {
    case kIndexUint8: return DXGI_FORMAT_R8_UINT;
    case kIndexUint16: return DXGI_FORMAT_R16_UINT;
    case kIndexUint32: return DXGI_FORMAT_R32_UINT;
    default:
      DCHECK(false) << "Unsupport IndexType: " << (int32_t)type;
      return DXGI_FORMAT_UNKNOWN;
  }
}

D3D11_BIND_FLAG TranslateBindTarget(uint32_t target) {
  DWORD flags = 0;
  if (target & kBindTargetShaderResource) {
    flags |= D3D11_BIND_SHADER_RESOURCE;
  }
  
  if (target & kBindTargetRenderTarget) {
    flags |= D3D11_BIND_RENDER_TARGET;
  }
  
  if (target & kBindTargetDepthStencil) {
    flags |= D3D11_BIND_DEPTH_STENCIL;
  }

  if (target & kBindTargetUnorderedAccess) {
    flags |= D3D11_BIND_UNORDERED_ACCESS;
  }

  if (target & kBindTargetVertexBuffer) {
    flags |= D3D11_BIND_VERTEX_BUFFER;
  }

  if (target & kBindTargetStreamOut) {
    flags |= D3D11_BIND_STREAM_OUTPUT;
  }
  return (D3D11_BIND_FLAG)flags;
}

D3D11_FILL_MODE TranslateFillMode(FillMode mode) {
  switch(mode) {
    case kWireFrame: return D3D11_FILL_WIREFRAME;
    case kSolid: return D3D11_FILL_SOLID;
    default: DCHECK(false); return (D3D11_FILL_MODE)0;
  }
}

FillMode TranslateD3DFillMode(D3D11_FILL_MODE mode) {
  switch(mode) {
    case D3D11_FILL_WIREFRAME: return kWireFrame;
    case D3D11_FILL_SOLID: return kSolid;
    default: DCHECK(false); return (FillMode)0;
  }
}

D3D11_CULL_MODE TranslateCullingMode(CullingMode mode) {
  switch (mode) {
    case kCullNone: return D3D11_CULL_NONE;
    case kCullFront: return D3D11_CULL_FRONT;
    case kCullBack: return D3D11_CULL_BACK;
    default: CHECK(false); return (D3D11_CULL_MODE)0;
  }
}

CullingMode TranslateD3DCullingMode(D3D11_CULL_MODE mode) {
  switch (mode) {
    case D3D11_CULL_NONE: return kCullNone;
    case D3D11_CULL_FRONT: return kCullFront;
    case D3D11_CULL_BACK: return kCullBack;
    default: CHECK(false); return (CullingMode)0;
  }
}

D3D11_TEXTURE_ADDRESS_MODE TranslateTexWrapMode(TexAddressMode mode) {
  switch (mode) {
    case TexAddressMode::kWrap: return D3D11_TEXTURE_ADDRESS_WRAP;
    case TexAddressMode::kMirror: return D3D11_TEXTURE_ADDRESS_MIRROR;
    case TexAddressMode::kClamp: return  D3D11_TEXTURE_ADDRESS_CLAMP;
    case TexAddressMode::kBorder: return D3D11_TEXTURE_ADDRESS_BORDER;
    case TexAddressMode::kMirrorOnce: return D3D11_TEXTURE_ADDRESS_BORDER;
    default:CHECK(false); return (D3D11_TEXTURE_ADDRESS_MODE)0;
  }
}

D3D11_BLEND TranslateBlending(Blending::Blend type) {
  switch (type) {
    case Blending::kZero:         return D3D11_BLEND_ZERO;
    case Blending::kOne:          return D3D11_BLEND_ONE;
    case Blending::kSrcColor:     return D3D11_BLEND_SRC_COLOR;
    case Blending::kSrcInvColor:  return D3D11_BLEND_INV_SRC_COLOR;
    case Blending::kSrcAlpha:     return D3D11_BLEND_SRC_ALPHA;
    case Blending::kSrcInvAlpha:  return D3D11_BLEND_INV_SRC_ALPHA;
    case Blending::kDestAlpha:    return D3D11_BLEND_DEST_ALPHA;
    case Blending::kDestInvAlpha: return D3D11_BLEND_INV_DEST_ALPHA;
    case Blending::kDestColor:    return D3D11_BLEND_DEST_COLOR;
    case Blending::kDestInvColor: return D3D11_BLEND_INV_DEST_COLOR;
    case Blending::kSrcAlphaSat:  return D3D11_BLEND_SRC_ALPHA_SAT;
    case Blending::kFactor:       return D3D11_BLEND_BLEND_FACTOR;
    case Blending::kInvFactor:    return D3D11_BLEND_INV_BLEND_FACTOR;
    case Blending::kSrc1Color:    return D3D11_BLEND_SRC1_COLOR;
    case Blending::kSrc1InvColor: return D3D11_BLEND_INV_SRC1_COLOR;
    case Blending::kSrc1Alpha:    return D3D11_BLEND_SRC1_ALPHA;
    case Blending::kSrc1InvAlpha: return D3D11_BLEND_INV_SRC1_ALPHA;
    default: CHECK(false); return (D3D11_BLEND)0;
  }
}

D3D11_BLEND_OP TranslateBlendingOper(Blending::Oper oper) {
  switch (oper) {
    case Blending::kAdd: return D3D11_BLEND_OP_ADD;
    case Blending::kSub: return D3D11_BLEND_OP_SUBTRACT;
    case Blending::kRevsub: return D3D11_BLEND_OP_REV_SUBTRACT;
    case Blending::kMin: return D3D11_BLEND_OP_MIN;
    case Blending::kMax: return D3D11_BLEND_OP_MAX;
    default: CHECK(false); return (D3D11_BLEND_OP)0;
  }
}

D3D11_COLOR_WRITE_ENABLE TranslateWriteMask(Blending::WriteMask mask) {
  switch (mask) {
    case Blending::kWriteR: return D3D11_COLOR_WRITE_ENABLE_RED;
    case Blending::kWriteG: return D3D11_COLOR_WRITE_ENABLE_GREEN;
    case Blending::kWriteB: return D3D11_COLOR_WRITE_ENABLE_BLUE;
    case Blending::kWriteA: return D3D11_COLOR_WRITE_ENABLE_ALPHA;
    case Blending::kWriteColor: return
        (D3D11_COLOR_WRITE_ENABLE)(D3D11_COLOR_WRITE_ENABLE_RED
                                   | D3D11_COLOR_WRITE_ENABLE_GREEN
                                   | D3D11_COLOR_WRITE_ENABLE_BLUE);
    case Blending::kWriteAll: return D3D11_COLOR_WRITE_ENABLE_ALL;
    default: CHECK(false); return (D3D11_COLOR_WRITE_ENABLE)0;
  }
}

D3D11_SRV_DIMENSION  GetViewDimensionFromTextureType(TexType type) {
  switch (type) {
    case TexType::k1D: return D3D11_SRV_DIMENSION_TEXTURE1D;
    case TexType::k1DArray: return D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
    case TexType::k2D: return D3D11_SRV_DIMENSION_TEXTURE2D;
    case TexType::k2DArray: return D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    case TexType::k2DMultiSample: return D3D11_SRV_DIMENSION_TEXTURE2DMS;
    case TexType::k2DArrayMultiSample: return D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
    case TexType::k3D: return D3D11_SRV_DIMENSION_TEXTURE3D;
    case TexType::kCubemap: return D3D11_SRV_DIMENSION_TEXTURECUBE;
    case TexType::kCubemapArray: return D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
    default: CHECK(false); return (D3D11_SRV_DIMENSION)0;
  }
}

D3D11_MAP TranslateMapType(MapType type) {
  switch (type) {
    case MapType::kReadOnly: return D3D11_MAP_READ;
    case MapType::kWrite: return D3D11_MAP_WRITE;
    case MapType::kReadWrite: return D3D11_MAP_READ_WRITE;
    case MapType::kWriteDiscard: return D3D11_MAP_WRITE_DISCARD;
    case MapType::kWriteNoOverwrite: return D3D11_MAP_WRITE_NO_OVERWRITE;
    default: CHECK(false); return (D3D11_MAP)0;
  }
}

D3D11_COMPARISON_FUNC TranslateCompareFunc(CompareFunc type) {
  switch (type) {
    case CompareFunc::kNever: return D3D11_COMPARISON_NEVER;
    case CompareFunc::kLess: return D3D11_COMPARISON_LESS;
    case CompareFunc::kEqual: return D3D11_COMPARISON_EQUAL;
    case CompareFunc::kLessEqual: return D3D11_COMPARISON_LESS_EQUAL;
    case CompareFunc::kGreater: return D3D11_COMPARISON_GREATER;
    case CompareFunc::kNotEqueal: return D3D11_COMPARISON_NOT_EQUAL;
    case CompareFunc::kGreaterEqual: return D3D11_COMPARISON_GREATER_EQUAL; 
    case CompareFunc::kAlways: return D3D11_COMPARISON_ALWAYS;
    default: CHECK(false); return (D3D11_COMPARISON_FUNC)0;
  }
}

D3D11_FILTER TranslateSamplerStateCompFilter(const SamplerState::Options& state) {
  DCHECK_NE(state.compare_func, CompareFunc::kNever);
  if (state.mag_filter == FilterMode::kPoint
      && state.min_filter == FilterMode::kPoint
      && state.mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
  } else if (state.mag_filter == FilterMode::kLinear
             && state.min_filter == FilterMode::kLinear
             && state.mip_filter == FilterMode::kLinear) {
    return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
  } else if (state.mag_filter == FilterMode::kPoint
             && state.min_filter == FilterMode::kLinear
             && state.mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
  } else if (state.mag_filter == FilterMode::kPoint
             && state.min_filter == FilterMode::kLinear
             && state.mip_filter == FilterMode::kLinear) {
    return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
  } else if (state.mag_filter == FilterMode::kLinear
             && state.min_filter == FilterMode::kPoint
             && state.mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
  } else if (state.mag_filter == FilterMode::kLinear
             && state.min_filter == FilterMode::kPoint
             && state.mip_filter == FilterMode::kLinear) {
    return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
  } else if (state.mag_filter == FilterMode::kLinear
             && state.min_filter == FilterMode::kLinear
             && state.mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
  } else {
    NOTREACHED();
    return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
  }
}

D3D11_FILTER TranslateSamplerStateFilter(const SamplerState::Options& state) {
  if (state.mag_filter == FilterMode::kPoint
      && state.min_filter == FilterMode::kPoint
      && state.mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_MIN_MAG_MIP_POINT;
  } else if (state.mag_filter == FilterMode::kLinear
             && state.min_filter == FilterMode::kLinear
             && state.mip_filter == FilterMode::kLinear) {
    return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  } else if (state.mag_filter == FilterMode::kPoint
             && state.min_filter == FilterMode::kLinear
             && state.mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
  } else if (state.mag_filter == FilterMode::kPoint
             && state.min_filter == FilterMode::kLinear
             && state.mip_filter == FilterMode::kLinear) {
    return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
  } else if (state.mag_filter == FilterMode::kLinear
             && state.min_filter == FilterMode::kPoint
             && state.mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
  } else if (state.mag_filter == FilterMode::kLinear
             && state.min_filter == FilterMode::kPoint
             && state.mip_filter == FilterMode::kLinear) {
    return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
  } else if (state.mag_filter == FilterMode::kLinear
             && state.min_filter == FilterMode::kLinear
             && state.mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
  } else {
    NOTREACHED();
    return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  }
}

D3D11_STENCIL_OP TranslateStencilOper(StencilOper oper) {
  switch (oper) {
    case StencilOper::kKeep: return D3D11_STENCIL_OP_KEEP;
    case StencilOper::kZero: return D3D11_STENCIL_OP_ZERO;
    case StencilOper::kReplace: return D3D11_STENCIL_OP_REPLACE;
    case StencilOper::kIncrSat: return D3D11_STENCIL_OP_INCR_SAT;
    case StencilOper::kDecrSat: return D3D11_STENCIL_OP_DECR_SAT;
    case StencilOper::kInvert: return D3D11_STENCIL_OP_INVERT;
    case StencilOper::kIncr: return D3D11_STENCIL_OP_INCR;
    case StencilOper::kDecr: return D3D11_STENCIL_OP_DECR;
    default: NOTREACHED();
      return D3D11_STENCIL_OP_KEEP;
  }
}
}  // namespace d3d11
}  // namespace azer
