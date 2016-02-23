#include "azer/util/images/dxutil.h"

#include "base/logging.h"
#include "azer/render/common.h"

namespace azer {
namespace detail {
uint32  TranslateFormat(int32 type) {
  switch (type) {
    case DXGI_FORMAT_R8G8B8A8_UNORM: return kRGBAn8;
    case DXGI_FORMAT_R32G32B32A32_FLOAT: return kRGBAf;
    case DXGI_FORMAT_B8G8R8A8_UNORM: return kBGRAn8;
    case DXGI_FORMAT_R8G8B8A8_UINT: return kRGBA8;
    case DXGI_FORMAT_R32G32B32A32_UINT: return kRGBA32;
    case DXGI_FORMAT_D24_UNORM_S8_UINT: return kDepth24Stencil8;
    case DXGI_FORMAT_R32_FLOAT: return kScalar;
    case DXGI_FORMAT_R32G32_FLOAT: return kVec2;
    case DXGI_FORMAT_R32G32B32_FLOAT: return kVec3;
    case DXGI_FORMAT_R32_SINT: return kInt;
    case DXGI_FORMAT_R32G32_SINT: return kIntVec2;
    case DXGI_FORMAT_R32G32B32_SINT: return kIntVec3;
    case DXGI_FORMAT_R32_UINT: return kUint;
    case DXGI_FORMAT_R32G32_UINT: return kUintVec2;
    case DXGI_FORMAT_R32G32B32_UINT: return kUintVec3;
    case DXGI_FORMAT_BC1_UNORM: return kDXBC1n;
    case DXGI_FORMAT_BC1_UNORM_SRGB: return kDXBC1nSRGB;
    case DXGI_FORMAT_BC4_UNORM: return kDXBC4n;
    case DXGI_FORMAT_BC4_SNORM: return kDXBC4s;
    case DXGI_FORMAT_BC2_UNORM: return kDXBC2n;
    case DXGI_FORMAT_BC2_UNORM_SRGB: return kDXBC2nSRGB;
    case DXGI_FORMAT_BC3_UNORM: return kDXBC3n;
    case DXGI_FORMAT_BC3_UNORM_SRGB: return kDXBC3nSRGB;
    case DXGI_FORMAT_BC5_UNORM: return kDXBC5n;
    case DXGI_FORMAT_BC5_SNORM: return kDXBC5s;
    case DXGI_FORMAT_BC7_UNORM: return kDXBC7n;
    case DXGI_FORMAT_BC7_UNORM_SRGB: return kDXBC7nSRGB;
    default:
      DCHECK(false) << "Unsupport type: " << (int32)type;
      return DXGI_FORMAT_UNKNOWN;
  }
}
}  // namespace detail
}  // namespace azer
