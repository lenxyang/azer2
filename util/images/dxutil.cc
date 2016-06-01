#include "azer/util/images/dxutil.h"

#include "base/logging.h"
#include "azer/render/common.h"
#include "azer/render/texture.h"

namespace azer {
namespace detail {
uint32_t  TranslateFormat(int32_t type) {
  switch (type) {
    case DXGI_FORMAT_R8G8B8A8_UNORM: return kTexRGBAn8;
    case DXGI_FORMAT_R32_FLOAT: return kTexR32;
    case DXGI_FORMAT_R32G32_FLOAT: return kTexRG32;
    case DXGI_FORMAT_R32G32B32_FLOAT: return kTexRGB32;
    case DXGI_FORMAT_R32G32B32A32_FLOAT: return kTexRGBAf;
    case DXGI_FORMAT_B8G8R8A8_UNORM: return kTexBGRAn8;
    case DXGI_FORMAT_R8G8B8A8_UINT: return kTexRGBA8;
    case DXGI_FORMAT_R32G32B32A32_UINT: return kTexRGBA32;
    case DXGI_FORMAT_D24_UNORM_S8_UINT: return kTexDepth24nStencil8u;
    case DXGI_FORMAT_BC1_UNORM: return kTexDXBC1n;
    case DXGI_FORMAT_BC1_UNORM_SRGB: return kTexDXBC1nSRGB;
    case DXGI_FORMAT_BC4_UNORM: return kTexDXBC4n;
    case DXGI_FORMAT_BC4_SNORM: return kTexDXBC4s;
    case DXGI_FORMAT_BC2_UNORM: return kTexDXBC2n;
    case DXGI_FORMAT_BC2_UNORM_SRGB: return kTexDXBC2nSRGB;
    case DXGI_FORMAT_BC3_UNORM: return kTexDXBC3n;
    case DXGI_FORMAT_BC3_UNORM_SRGB: return kTexDXBC3nSRGB;
    case DXGI_FORMAT_BC5_UNORM: return kTexDXBC5n;
    case DXGI_FORMAT_BC5_SNORM: return kTexDXBC5s;
    case DXGI_FORMAT_BC7_UNORM: return kTexDXBC7n;
    case DXGI_FORMAT_BC7_UNORM_SRGB: return kTexDXBC7nSRGB;
    default:
      DCHECK(false) << "Unsupport type: " << (int32_t)type;
      return DXGI_FORMAT_UNKNOWN;
  }
}
}  // namespace detail
}  // namespace azer
