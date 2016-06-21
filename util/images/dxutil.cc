#include "azer/util/images/dxutil.h"

#include "base/logging.h"
#include "azer/render/common.h"
#include "azer/render/texture.h"

namespace azer {
namespace detail {
TexFormat  TranslateFormat(int32_t type) {
  switch (type) {
    case DXGI_FORMAT_B8G8R8A8_UNORM:     return TexFormat::kBGRA8UNorm;
    case DXGI_FORMAT_R8G8B8A8_UNORM:     return TexFormat::kRGBA8UNorm;
    case DXGI_FORMAT_R8G8B8A8_UINT:      return TexFormat::kRGBA8Uint;
    case DXGI_FORMAT_R32_FLOAT:          return TexFormat::kR32Float;
    case DXGI_FORMAT_R32G32_FLOAT:       return TexFormat::kRG32Float;
    case DXGI_FORMAT_R32G32B32_FLOAT:    return TexFormat::kRGB32Float;
    case DXGI_FORMAT_R32G32B32A32_FLOAT: return TexFormat::kRGBA32Float;
    case DXGI_FORMAT_R32G32B32A32_UINT:  return TexFormat::kRGBA32Uint;
    case DXGI_FORMAT_D24_UNORM_S8_UINT:  return TexFormat::kD24UNormS8Uint;
    case DXGI_FORMAT_BC1_UNORM:          return TexFormat::kDXBC1n;
    case DXGI_FORMAT_BC1_UNORM_SRGB:     return TexFormat::kDXBC1nSRGB;
    case DXGI_FORMAT_BC4_UNORM:          return TexFormat::kDXBC4n;
    case DXGI_FORMAT_BC4_SNORM:          return TexFormat::kDXBC4s;
    case DXGI_FORMAT_BC2_UNORM:          return TexFormat::kDXBC2n;
    case DXGI_FORMAT_BC2_UNORM_SRGB:     return TexFormat::kDXBC2nSRGB;
    case DXGI_FORMAT_BC3_UNORM:          return TexFormat::kDXBC3n;
    case DXGI_FORMAT_BC3_UNORM_SRGB:     return TexFormat::kDXBC3nSRGB;
    case DXGI_FORMAT_BC5_UNORM:          return TexFormat::kDXBC5n;
    case DXGI_FORMAT_BC5_SNORM:          return TexFormat::kDXBC5s;
    case DXGI_FORMAT_BC7_UNORM:          return TexFormat::kDXBC7n;
    case DXGI_FORMAT_BC7_UNORM_SRGB:     return TexFormat::kDXBC7nSRGB;
    default:
      DCHECK(false) << "Unsupport type: " << (int32_t)type;
      return TexFormat::kUndefined;
  }
}
}  // namespace detail
}  // namespace azer
