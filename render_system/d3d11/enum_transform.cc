#include "azer/render_system/d3d11/enum_transform.h"

#include "base/logging.h"

namespace azer {
namespace d3d11 {

D3D11_USAGE TranslateUsage(GraphicBuffer::Usage usage) {
  switch(usage) {
    case GraphicBuffer::kDefault: return  D3D11_USAGE_DEFAULT;
    case GraphicBuffer::kGPUReayOnly: return D3D11_USAGE_IMMUTABLE;
    case GraphicBuffer::kDynamic: return D3D11_USAGE_DYNAMIC;
    case GraphicBuffer::kStaging: return D3D11_USAGE_STAGING;
    default:
      DCHECK(false) << "Unsupport usage: " << (int32)usage;
      return D3D11_USAGE_DEFAULT;
  }
}

D3D11_CPU_ACCESS_FLAG TranslateCPUAccess(CPUAccess access) {
  switch (access) {
    case kCPUNoAccess: return (D3D11_CPU_ACCESS_FLAG)0x0;
    case kCPUWrite: return D3D11_CPU_ACCESS_WRITE;
    case kCPURead: return D3D11_CPU_ACCESS_READ;
    case kCPUAccess:
      return (D3D11_CPU_ACCESS_FLAG)(D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ);
    default:
      DCHECK(false) << "Unsupport CPUAccess: " << (int32)access;
      return (D3D11_CPU_ACCESS_FLAG)(D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ);
  }
}

DXGI_FORMAT TranslateFormat(DataFormat type) {
  switch (type) {
    case kRGBAn8: return DXGI_FORMAT_R8G8B8A8_UNORM;
    case kRGBAf: return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case kBGRAn8: return DXGI_FORMAT_B8G8R8A8_UNORM;
    case kRGBA8: NOTREACHED(); return DXGI_FORMAT_R8G8B8A8_UINT;
    case kRGBA32: NOTREACHED(); return DXGI_FORMAT_R32G32B32A32_UINT;
    case kRGBAn32: return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case kDepth24Stencil8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case kScalar: return DXGI_FORMAT_D32_FLOAT;
    case kVec2: return DXGI_FORMAT_R32G32_FLOAT;
    case kVec3: return DXGI_FORMAT_R32G32B32_FLOAT;
    case kVec4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case kInt: return DXGI_FORMAT_R32_SINT;
    case kIntVec2: return DXGI_FORMAT_R32G32_SINT;
    case kIntVec3: return DXGI_FORMAT_R32G32B32_SINT;
    case kIntVec4: return DXGI_FORMAT_R32G32B32A32_SINT;
    case kUint: return DXGI_FORMAT_R32_UINT;
    case kUintVec2: return DXGI_FORMAT_R32G32_UINT;
    case kUintVec3: return DXGI_FORMAT_R32G32B32_UINT;
    case kUintVec4: return DXGI_FORMAT_R32G32B32A32_UINT;
    case kBool: return DXGI_FORMAT_R32_SINT;
    case kBoolVec2: return DXGI_FORMAT_R32G32_SINT;
    case kBoolVec3: return DXGI_FORMAT_R32G32B32_SINT;
    case kBoolVec4: return DXGI_FORMAT_R32G32B32A32_SINT;
    default:
      DCHECK(false) << "Unsupport type: " << (int32)type;
      return DXGI_FORMAT_UNKNOWN;
  }
}

DataFormat TranslateD3DFormat(DXGI_FORMAT type) {
  switch (type) {
    case DXGI_FORMAT_B8G8R8A8_UNORM: return kBGRAn8;
    case DXGI_FORMAT_R8G8B8A8_UNORM: return kRGBAn8;
    case DXGI_FORMAT_R32G32B32A32_UINT: return kRGBAn32;
    case DXGI_FORMAT_R32G32B32A32_FLOAT: return kRGBAf;
    case DXGI_FORMAT_D24_UNORM_S8_UINT: return kDepth24Stencil8;
    case DXGI_FORMAT_R32G32_FLOAT: return kVec2;
    case DXGI_FORMAT_R32G32B32_FLOAT: return kVec3;
    default:
      DCHECK(false) << "Unsupport type: " << (int32)type;
      return (DataFormat)0;
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
      DCHECK(false) << "Unsupport type: " << (int32)topology;
      return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
  }
}

DXGI_FORMAT TranslateIndexType(IndicesData::IndexType type) {
  switch(type) {
    case IndicesData::kUint8: return DXGI_FORMAT_R8_UINT;
    case IndicesData::kUint16: return DXGI_FORMAT_R16_UINT;
    case IndicesData::kUint32: return DXGI_FORMAT_R32_UINT;
    default:
      DCHECK(false) << "Unsupport IndexType: " << (int32)type;
      return DXGI_FORMAT_UNKNOWN;
  }
}

D3D11_CLEAR_FLAG TranslateDepthAndStencilClearFlag(
    azer::DepthBuffer::ClearFlag clear) {
  DWORD flags = 0;
  if (clear & azer::DepthBuffer::kClearDepth) {
    flags |= D3D11_CLEAR_DEPTH;
  }

  if (clear & azer::DepthBuffer::kClearStencil) {
    flags |= D3D11_CLEAR_STENCIL;
  }

  return (D3D11_CLEAR_FLAG)flags;
}

D3D11_BIND_FLAG TranslateBindTarget(Texture::BindTarget target) {
  DWORD flags = 0;
  if (target & Texture::kShaderResource)
    flags |= D3D11_BIND_SHADER_RESOURCE;
  
  if (target & Texture::kRenderTarget)
    flags |= D3D11_BIND_RENDER_TARGET;
  
  if (target & Texture::kDepthStencil)
    flags |= D3D11_BIND_DEPTH_STENCIL;
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

D3D11_TEXTURE_ADDRESS_MODE TranslateTexWrapMode(Texture::WrapMode mode) {
  switch (mode) {
    case Texture::kWrap: return D3D11_TEXTURE_ADDRESS_WRAP;
    case Texture::kMirror: return D3D11_TEXTURE_ADDRESS_MIRROR;
    case Texture::kClamp: return  D3D11_TEXTURE_ADDRESS_CLAMP;
    case Texture::kBorder: return D3D11_TEXTURE_ADDRESS_BORDER;
    case Texture::kMirrorOnce: return D3D11_TEXTURE_ADDRESS_BORDER;
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

D3D11_SRV_DIMENSION  GetViewDimensionFromTextureType(Texture::Type type) {
  switch (type) {
    case Texture::k1D: return D3D11_SRV_DIMENSION_TEXTURE1D;
    case Texture::k1DArray: return D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
    case Texture::k2D: return D3D11_SRV_DIMENSION_TEXTURE2D;
    case Texture::k2DArray: return D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    case Texture::k2DMultiSample: return D3D11_SRV_DIMENSION_TEXTURE2DMS;
    case Texture::k2DArrayMultiSample: return D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
    case Texture::k3D: return D3D11_SRV_DIMENSION_TEXTURE3D;
    case Texture::kCubemap: return D3D11_SRV_DIMENSION_TEXTURECUBE;
    case Texture::kCubemapArray: return D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
    default: CHECK(false); return (D3D11_SRV_DIMENSION)0;
  }
}

D3D11_MAP TranslateMapType(MapType type) {
  switch (type) {
    case kReadOnly: return D3D11_MAP_READ;
    case kWrite: return D3D11_MAP_WRITE;
    case kReadWrite: return D3D11_MAP_READ_WRITE;
    case kWriteDiscard: return D3D11_MAP_WRITE_DISCARD;
    case kWriteNoOverwrite: return D3D11_MAP_WRITE_NO_OVERWRITE;
    default: CHECK(false); return (D3D11_MAP)0;
  }
}

D3D11_COMPARISON_FUNC TranslateCompareFunc(CompareFunc::Type type) {
  switch (type) {
    case CompareFunc::kNever: return D3D11_COMPARISON_NEVER;
    case CompareFunc::kLess: return D3D11_COMPARISON_LESS;
    case CompareFunc::kEqual: return D3D11_COMPARISON_EQUAL;
    case CompareFunc::kLessEqual: return D3D11_COMPARISON_LESS_EQUAL;
    case CompareFunc::kGreater: return D3D11_COMPARISON_GREATER;
    case CompareFunc::kNotEqueal: return D3D11_COMPARISON_NOT_EQUAL;
    case CompareFunc::kGreaterEqua: return D3D11_COMPARISON_GREATER_EQUAL; 
    case CompareFunc::kAlways: return D3D11_COMPARISON_ALWAYS;
    default: CHECK(false); return (D3D11_COMPARISON_FUNC)0;
  }
}

D3D11_FILTER TranslateSamplerState(const Texture::SamplerState& state) {
  if (state.mag_filter == Texture::kPoint && state.min_filter == Texture::kPoint
      && state.mip_filter == Texture::kPoint) {
    return D3D11_FILTER_MIN_MAG_MIP_POINT;
  } else if (state.mag_filter == Texture::kLinear
             && state.min_filter == Texture::kLinear
             && state.mip_filter == Texture::kLinear) {
    return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  } else if (state.mag_filter == Texture::kPoint
             && state.min_filter == Texture::kLinear
             && state.mip_filter == Texture::kPoint) {
    return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
  } else if (state.mag_filter == Texture::kPoint
             && state.min_filter == Texture::kLinear
             && state.mip_filter == Texture::kLinear) {
    return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
  } else if (state.mag_filter == Texture::kLinear
             && state.min_filter == Texture::kPoint
             && state.mip_filter == Texture::kPoint) {
    return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
  } else if (state.mag_filter == Texture::kLinear
             && state.min_filter == Texture::kPoint
             && state.mip_filter == Texture::kLinear) {
    return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
  } else if (state.mag_filter == Texture::kLinear
             && state.min_filter == Texture::kLinear
             && state.mip_filter == Texture::kPoint) {
    return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
  } else {
    NOTREACHED();
    return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  }
}
}  // namespace d3d11
}  // namespace azer
