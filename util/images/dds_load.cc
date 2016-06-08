#include "azer/util/images/image.h"

#include "base/files/file_path.h"
#include "base/files/file_util.h"

#include "azer/render/common.h"
#include "azer/util/images/color.h"
#include "azer/util/images/dds.h"
#include "azer/util/images/dxutil.h"
                                  
namespace azer {
namespace {
//--------------------------------------------------------------------------------------
#define ISBITMASK(r,g,b,a) (ddpf.RBitMask == r && ddpf.GBitMask == g && ddpf.BBitMask == b && ddpf.ABitMask == a)

int32_t GetDXGIFormat(const DDS_PIXELFORMAT& ddpf) {
  using namespace detail;
  if (ddpf.flags & DDS_RGB) {
    // Note that sRGB formats are written using the "DX10" extended header

    switch (ddpf.RGBBitCount) {
      case 32:
        if (ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0xff000000)) {
          return DXGI_FORMAT_R8G8B8A8_UNORM;
        }

        if (ISBITMASK(0x00ff0000,0x0000ff00,0x000000ff,0xff000000)) {
          return DXGI_FORMAT_B8G8R8A8_UNORM;
        }

        if (ISBITMASK(0x00ff0000,0x0000ff00,0x000000ff,0x00000000)) {
          return DXGI_FORMAT_B8G8R8X8_UNORM;
        }

        // No DXGI format maps to ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0x00000000) aka D3DFMT_X8B8G8R8

        // Note that many common DDS reader/writers (including D3DX) swap the
        // the RED/BLUE masks for 10:10:10:2 formats. We assume
        // below that the 'backwards' header mask is being used since it is most
        // likely written by D3DX. The more robust solution is to use the 'DX10'
        // header extension and specify the DXGI_FORMAT_R10G10B10A2_UNORM format directly

        // For 'correct' writers, this should be 0x000003ff,0x000ffc00,0x3ff00000 for RGB data
        if (ISBITMASK(0x3ff00000,0x000ffc00,0x000003ff,0xc0000000)) {
          return DXGI_FORMAT_R10G10B10A2_UNORM;
        }

        // No DXGI format maps to ISBITMASK(0x000003ff,0x000ffc00,0x3ff00000,0xc0000000) aka D3DFMT_A2R10G10B10

        if (ISBITMASK(0x0000ffff,0xffff0000,0x00000000,0x00000000)) {
          return DXGI_FORMAT_R16G16_UNORM;
        }

        if (ISBITMASK(0xffffffff,0x00000000,0x00000000,0x00000000)) {
          // Only 32-bit color channel format in D3D9 was R32F
          return DXGI_FORMAT_R32_FLOAT; // D3DX writes this out as a FourCC of 114
        }
        break;

      case 24:
        // No 24bpp DXGI formats aka D3DFMT_R8G8B8
        break;

      case 16:
        if (ISBITMASK(0x7c00,0x03e0,0x001f,0x8000)) {
          return DXGI_FORMAT_B5G5R5A1_UNORM;
        }
        if (ISBITMASK(0xf800,0x07e0,0x001f,0x0000)) {
          return DXGI_FORMAT_B5G6R5_UNORM;
        }

        // No DXGI format maps to ISBITMASK(0x7c00,0x03e0,0x001f,0x0000) aka D3DFMT_X1R5G5B5

        if (ISBITMASK(0x0f00,0x00f0,0x000f,0xf000)) {
          return DXGI_FORMAT_B4G4R4A4_UNORM;
        }

        // No DXGI format maps to ISBITMASK(0x0f00,0x00f0,0x000f,0x0000) aka D3DFMT_X4R4G4B4

        // No 3:3:2, 3:3:2:8, or paletted DXGI formats aka D3DFMT_A8R3G3B2, D3DFMT_R3G3B2, D3DFMT_P8, D3DFMT_A8P8, etc.
        break;
    }
  } else if (ddpf.flags & DDS_LUMINANCE) {
    if (8 == ddpf.RGBBitCount) {
      if (ISBITMASK(0x000000ff,0x00000000,0x00000000,0x00000000)) {
        return DXGI_FORMAT_R8_UNORM; // D3DX10/11 writes this out as DX10 extension
      }

      // No DXGI format maps to ISBITMASK(0x0f,0x00,0x00,0xf0) aka D3DFMT_A4L4
    }

    if (16 == ddpf.RGBBitCount) {
      if (ISBITMASK(0x0000ffff,0x00000000,0x00000000,0x00000000)) {
        return DXGI_FORMAT_R16_UNORM; // D3DX10/11 writes this out as DX10 extension
      }
      if (ISBITMASK(0x000000ff,0x00000000,0x00000000,0x0000ff00)) {
        return DXGI_FORMAT_R8G8_UNORM; // D3DX10/11 writes this out as DX10 extension
      }
    }
  } else if (ddpf.flags & DDS_ALPHA) {
    if (8 == ddpf.RGBBitCount) {
      return DXGI_FORMAT_A8_UNORM;
    }
  } else if (ddpf.flags & DDS_BUMPDUDV) {
    if (16 == ddpf.RGBBitCount) {
      if (ISBITMASK(0x00ff, 0xff00, 0x0000, 0x0000)) {
        return DXGI_FORMAT_R8G8_SNORM; // D3DX10/11 writes this out as DX10 extension
      }
    }

    if (32 == ddpf.RGBBitCount) {
      if (ISBITMASK(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000)) {
        return DXGI_FORMAT_R8G8B8A8_SNORM; // D3DX10/11 writes this out as DX10 extension
      }
      if (ISBITMASK(0x0000ffff, 0xffff0000, 0x00000000, 0x00000000)) {
        return DXGI_FORMAT_R16G16_SNORM; // D3DX10/11 writes this out as DX10 extension
      }

      // No DXGI format maps to ISBITMASK(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000) aka D3DFMT_A2W10V10U10
    }
  } else if (ddpf.flags & DDS_FOURCC) {
    if (MAKEFOURCC('D', 'X', 'T', '1') == ddpf.fourCC) {
      return DXGI_FORMAT_BC1_UNORM;
    }
    if (MAKEFOURCC('D', 'X', 'T', '3') == ddpf.fourCC) {
      return DXGI_FORMAT_BC2_UNORM;
    }
    if (MAKEFOURCC('D', 'X', 'T', '5') == ddpf.fourCC) {
      return DXGI_FORMAT_BC3_UNORM;
    }

    // While pre-multiplied alpha isn't directly supported by the DXGI formats,
    // they are basically the same as these BC formats so they can be mapped
    if (MAKEFOURCC('D', 'X', 'T', '2') == ddpf.fourCC) {
      return DXGI_FORMAT_BC2_UNORM;
    }
    if (MAKEFOURCC('D', 'X', 'T', '4') == ddpf.fourCC) {
      return DXGI_FORMAT_BC3_UNORM;
    }

    if (MAKEFOURCC('A', 'T', 'I', '1') == ddpf.fourCC) {
      return DXGI_FORMAT_BC4_UNORM;
    }
    if (MAKEFOURCC('B', 'C', '4', 'U') == ddpf.fourCC) {
      return DXGI_FORMAT_BC4_UNORM;
    }
    if (MAKEFOURCC('B', 'C', '4', 'S') == ddpf.fourCC) {
      return DXGI_FORMAT_BC4_SNORM;
    }

    if (MAKEFOURCC('A', 'T', 'I', '2') == ddpf.fourCC) {
      return DXGI_FORMAT_BC5_UNORM;
    }
    if (MAKEFOURCC('B', 'C', '5', 'U') == ddpf.fourCC) {
      return DXGI_FORMAT_BC5_UNORM;
    }
    if (MAKEFOURCC('B', 'C', '5', 'S') == ddpf.fourCC) {
      return DXGI_FORMAT_BC5_SNORM;
    }

    // BC6H and BC7 are written using the "DX10" extended header

    if (MAKEFOURCC('R', 'G', 'B', 'G') == ddpf.fourCC) {
      return DXGI_FORMAT_R8G8_B8G8_UNORM;
    }
    if (MAKEFOURCC('G', 'R', 'G', 'B') == ddpf.fourCC) {
      return DXGI_FORMAT_G8R8_G8B8_UNORM;
    }

    if (MAKEFOURCC('Y','U','Y','2') == ddpf.fourCC) {
      return DXGI_FORMAT_YUY2;
    }

    // Check for D3DFORMAT enums being set here
    switch(ddpf.fourCC) {
      case 36: // D3DFMT_A16B16G16R16
        return DXGI_FORMAT_R16G16B16A16_UNORM;

      case 110: // D3DFMT_Q16W16V16U16
        return DXGI_FORMAT_R16G16B16A16_SNORM;

      case 111: // D3DFMT_R16F
        return DXGI_FORMAT_R16_FLOAT;

      case 112: // D3DFMT_G16R16F
        return DXGI_FORMAT_R16G16_FLOAT;

      case 113: // D3DFMT_A16B16G16R16F
        return DXGI_FORMAT_R16G16B16A16_FLOAT;

      case 114: // D3DFMT_R32F
        return DXGI_FORMAT_R32_FLOAT;

      case 115: // D3DFMT_G32R32F
        return DXGI_FORMAT_R32G32_FLOAT;

      case 116: // D3DFMT_A32B32G32R32F
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
    }
  }

  return DXGI_FORMAT_UNKNOWN;
}

uint32_t BitsPerPixel(uint32_t fmt) {
  switch(fmt) {
    case detail::DXGI_FORMAT_R32G32B32A32_TYPELESS:
    case detail::DXGI_FORMAT_R32G32B32A32_FLOAT:
    case detail::DXGI_FORMAT_R32G32B32A32_UINT:
    case detail::DXGI_FORMAT_R32G32B32A32_SINT:
      return 128;

    case detail::DXGI_FORMAT_R32G32B32_TYPELESS:
    case detail::DXGI_FORMAT_R32G32B32_FLOAT:
    case detail::DXGI_FORMAT_R32G32B32_UINT:
    case detail::DXGI_FORMAT_R32G32B32_SINT:
      return 96;

    case detail::DXGI_FORMAT_R16G16B16A16_TYPELESS:
    case detail::DXGI_FORMAT_R16G16B16A16_FLOAT:
    case detail::DXGI_FORMAT_R16G16B16A16_UNORM:
    case detail::DXGI_FORMAT_R16G16B16A16_UINT:
    case detail::DXGI_FORMAT_R16G16B16A16_SNORM:
    case detail::DXGI_FORMAT_R16G16B16A16_SINT:
    case detail::DXGI_FORMAT_R32G32_TYPELESS:
    case detail::DXGI_FORMAT_R32G32_FLOAT:
    case detail::DXGI_FORMAT_R32G32_UINT:
    case detail::DXGI_FORMAT_R32G32_SINT:
    case detail::DXGI_FORMAT_R32G8X24_TYPELESS:
    case detail::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    case detail::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case detail::DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    case detail::DXGI_FORMAT_Y416:
    case detail::DXGI_FORMAT_Y210:
    case detail::DXGI_FORMAT_Y216:
      return 64;

    case detail::DXGI_FORMAT_R10G10B10A2_TYPELESS:
    case detail::DXGI_FORMAT_R10G10B10A2_UNORM:
    case detail::DXGI_FORMAT_R10G10B10A2_UINT:
    case detail::DXGI_FORMAT_R11G11B10_FLOAT:
    case detail::DXGI_FORMAT_R8G8B8A8_TYPELESS:
    case detail::DXGI_FORMAT_R8G8B8A8_UNORM:
    case detail::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case detail::DXGI_FORMAT_R8G8B8A8_UINT:
    case detail::DXGI_FORMAT_R8G8B8A8_SNORM:
    case detail::DXGI_FORMAT_R8G8B8A8_SINT:
    case detail::DXGI_FORMAT_R16G16_TYPELESS:
    case detail::DXGI_FORMAT_R16G16_FLOAT:
    case detail::DXGI_FORMAT_R16G16_UNORM:
    case detail::DXGI_FORMAT_R16G16_UINT:
    case detail::DXGI_FORMAT_R16G16_SNORM:
    case detail::DXGI_FORMAT_R16G16_SINT:
    case detail::DXGI_FORMAT_R32_TYPELESS:
    case detail::DXGI_FORMAT_D32_FLOAT:
    case detail::DXGI_FORMAT_R32_FLOAT:
    case detail::DXGI_FORMAT_R32_UINT:
    case detail::DXGI_FORMAT_R32_SINT:
    case detail::DXGI_FORMAT_R24G8_TYPELESS:
    case detail::DXGI_FORMAT_D24_UNORM_S8_UINT:
    case detail::DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case detail::DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    case detail::DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    case detail::DXGI_FORMAT_R8G8_B8G8_UNORM:
    case detail::DXGI_FORMAT_G8R8_G8B8_UNORM:
    case detail::DXGI_FORMAT_B8G8R8A8_UNORM:
    case detail::DXGI_FORMAT_B8G8R8X8_UNORM:
    case detail::DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    case detail::DXGI_FORMAT_B8G8R8A8_TYPELESS:
    case detail::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
    case detail::DXGI_FORMAT_B8G8R8X8_TYPELESS:
    case detail::DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
    case detail::DXGI_FORMAT_AYUV:
    case detail::DXGI_FORMAT_Y410:
    case detail::DXGI_FORMAT_YUY2:
      return 32;

    case detail::DXGI_FORMAT_P010:
    case detail::DXGI_FORMAT_P016:
      return 24;

    case detail::DXGI_FORMAT_R8G8_TYPELESS:
    case detail::DXGI_FORMAT_R8G8_UNORM:
    case detail::DXGI_FORMAT_R8G8_UINT:
    case detail::DXGI_FORMAT_R8G8_SNORM:
    case detail::DXGI_FORMAT_R8G8_SINT:
    case detail::DXGI_FORMAT_R16_TYPELESS:
    case detail::DXGI_FORMAT_R16_FLOAT:
    case detail::DXGI_FORMAT_D16_UNORM:
    case detail::DXGI_FORMAT_R16_UNORM:
    case detail::DXGI_FORMAT_R16_UINT:
    case detail::DXGI_FORMAT_R16_SNORM:
    case detail::DXGI_FORMAT_R16_SINT:
    case detail::DXGI_FORMAT_B5G6R5_UNORM:
    case detail::DXGI_FORMAT_B5G5R5A1_UNORM:
    case detail::DXGI_FORMAT_A8P8:
    case detail::DXGI_FORMAT_B4G4R4A4_UNORM:
      return 16;

    case detail::DXGI_FORMAT_NV12:
    case detail::DXGI_FORMAT_420_OPAQUE:
    case detail::DXGI_FORMAT_NV11:
      return 12;

    case detail::DXGI_FORMAT_R8_TYPELESS:
    case detail::DXGI_FORMAT_R8_UNORM:
    case detail::DXGI_FORMAT_R8_UINT:
    case detail::DXGI_FORMAT_R8_SNORM:
    case detail::DXGI_FORMAT_R8_SINT:
    case detail::DXGI_FORMAT_A8_UNORM:
    case detail::DXGI_FORMAT_AI44:
    case detail::DXGI_FORMAT_IA44:
    case detail::DXGI_FORMAT_P8:
      return 8;

    case detail::DXGI_FORMAT_R1_UNORM:
      return 1;

    case detail::DXGI_FORMAT_BC1_TYPELESS:
    case detail::DXGI_FORMAT_BC1_UNORM:
    case detail::DXGI_FORMAT_BC1_UNORM_SRGB:
    case detail::DXGI_FORMAT_BC4_TYPELESS:
    case detail::DXGI_FORMAT_BC4_UNORM:
    case detail::DXGI_FORMAT_BC4_SNORM:
      return 4;

    case detail::DXGI_FORMAT_BC2_TYPELESS:
    case detail::DXGI_FORMAT_BC2_UNORM:
    case detail::DXGI_FORMAT_BC2_UNORM_SRGB:
    case detail::DXGI_FORMAT_BC3_TYPELESS:
    case detail::DXGI_FORMAT_BC3_UNORM:
    case detail::DXGI_FORMAT_BC3_UNORM_SRGB:
    case detail::DXGI_FORMAT_BC5_TYPELESS:
    case detail::DXGI_FORMAT_BC5_UNORM:
    case detail::DXGI_FORMAT_BC5_SNORM:
    case detail::DXGI_FORMAT_BC6H_TYPELESS:
    case detail::DXGI_FORMAT_BC6H_UF16:
    case detail::DXGI_FORMAT_BC6H_SF16:
    case detail::DXGI_FORMAT_BC7_TYPELESS:
    case detail::DXGI_FORMAT_BC7_UNORM:
    case detail::DXGI_FORMAT_BC7_UNORM_SRGB:
      return 8;
    default:
      return 0;
  }
}

int32_t GetDDSDetail(const DDS_HEADER& head, int32_t* height, int32_t* depth,
                     int32_t* arraysize, uint32_t* format) {
  *height = head.height;
  *depth = head.depth;
  *arraysize = 1;
  if ((head.ddspf.flags & DDS_FOURCC) &&
      (MAKEFOURCC('D', 'X', '1', '0') == head.ddspf.fourCC)) {
    auto d3d10ext = reinterpret_cast<const DDS_HEADER_DXT10*>(
        (const char*)&head + sizeof(DDS_HEADER));

    *arraysize = d3d10ext->arraySize;
    if (*arraysize == 0) {
      // return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
      return kUnkonwnTexType;
    }

    if (BitsPerPixel(d3d10ext->dxgiFormat) == 0) {
      // return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
      return kUnkonwnTexType;
    }
           
    *format = d3d10ext->dxgiFormat;
    switch (d3d10ext->resourceDimension) {
      case DDS_DIMENSION_TEXTURE1D:
        // D3DX writes 1D textures with a fixed Height of 1
        if ((head.flags & DDS_HEIGHT) && *height != 1) {
          // return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
          return kUnkonwnTexType;
        }
        *height = *depth = 1;
        return kTex1D;
      case DDS_DIMENSION_TEXTURE2D:
        *depth = 1;
        if (d3d10ext->miscFlag & DDS_RESOURCE_MISC_TEXTURECUBE) {
          *arraysize *= 6;
          return kTexCubemap;
        } else {
          return kTex2D;
        }
      case DDS_DIMENSION_TEXTURE3D:
        if (!(head.flags & DDS_HEADER_FLAGS_VOLUME)) {
          // return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
          return kUnkonwnTexType;
        }

        if (*arraysize > 1) {
          // return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
          return kUnkonwnTexType;
        }
        return kTex3D;
      default:
        // return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
        return kUnkonwnTexType;
    }
  } else {
    *format = GetDXGIFormat(head.ddspf);
    if (*format == detail::DXGI_FORMAT_UNKNOWN) {
      // return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
      return kUnkonwnTexType;
    }

    if (head.flags & DDS_HEADER_FLAGS_VOLUME) {
      // resDim = D3D11_RESOURCE_DIMENSION_TEXTURE3D;
      return kTex3D;
    } else {
      *depth = 1;
      if (head.caps2 & DDS_CUBEMAP) {
        // We require all six faces to be defined
        if ((head.caps2 & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES) {
          // return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
          return kUnkonwnTexType;
        }
        *arraysize = 6;
        return kTexCubemap;
      } else {
        // resDim = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
        return kTex2D;
      }
    }
  }
}

void GetSurfaceInfo(uint32_t width, uint32_t height, uint32_t fmt,
                    uint32_t* outNumBytes, uint32_t* outRowBytes,
                    uint32_t* outNumRows) {
  using namespace detail;
  uint32_t numBytes = 0;
  uint32_t rowBytes = 0;
  uint32_t numRows = 0;

  bool bc = false;
  bool packed = false;
  bool planar = false;
  uint32_t bpe = 0;
  switch (fmt) {
    case DXGI_FORMAT_BC1_TYPELESS:
    case DXGI_FORMAT_BC1_UNORM:
    case DXGI_FORMAT_BC1_UNORM_SRGB:
    case DXGI_FORMAT_BC4_TYPELESS:
    case DXGI_FORMAT_BC4_UNORM:
    case DXGI_FORMAT_BC4_SNORM:
      bc=true;
      bpe = 8;
      break;

    case DXGI_FORMAT_BC2_TYPELESS:
    case DXGI_FORMAT_BC2_UNORM:
    case DXGI_FORMAT_BC2_UNORM_SRGB:
    case DXGI_FORMAT_BC3_TYPELESS:
    case DXGI_FORMAT_BC3_UNORM:
    case DXGI_FORMAT_BC3_UNORM_SRGB:
    case DXGI_FORMAT_BC5_TYPELESS:
    case DXGI_FORMAT_BC5_UNORM:
    case DXGI_FORMAT_BC5_SNORM:
    case DXGI_FORMAT_BC6H_TYPELESS:
    case DXGI_FORMAT_BC6H_UF16:
    case DXGI_FORMAT_BC6H_SF16:
    case DXGI_FORMAT_BC7_TYPELESS:
    case DXGI_FORMAT_BC7_UNORM:
    case DXGI_FORMAT_BC7_UNORM_SRGB:
      bc = true;
      bpe = 16;
      break;

    case DXGI_FORMAT_R8G8_B8G8_UNORM:
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
    case DXGI_FORMAT_YUY2:
      packed = true;
      bpe = 4;
      break;

    case DXGI_FORMAT_Y210:
    case DXGI_FORMAT_Y216:
      packed = true;
      bpe = 8;
      break;

    case DXGI_FORMAT_NV12:
    case DXGI_FORMAT_420_OPAQUE:
      planar = true;
      bpe = 2;
      break;

    case DXGI_FORMAT_P010:
    case DXGI_FORMAT_P016:
      planar = true;
      bpe = 4;
      break;
  }

  if (bc) {
    uint32_t numBlocksWide = 0;
    if (width > 0) {
      numBlocksWide = std::max<uint32_t>(1, (width + 3) / 4);
    }
    uint32_t numBlocksHigh = 0;
    if (height > 0) {
      numBlocksHigh = std::max<uint32_t>(1, (height + 3) / 4);
    }
    rowBytes = numBlocksWide * bpe;
    numRows = numBlocksHigh;
    numBytes = rowBytes * numBlocksHigh;
  } else if (packed) {
    rowBytes = ((width + 1) >> 1) * bpe;
    numRows = height;
    numBytes = rowBytes * height;
  } else if (fmt == DXGI_FORMAT_NV11) {
    rowBytes = ((width + 3) >> 2) * 4;
    // Direct3D makes this simplifying assumption, although it is larger than the 4:1:1 data
    numRows = height * 2; 
    numBytes = rowBytes * numRows;
  } else if (planar) {
    rowBytes = ((width + 1) >> 1) * bpe;
    numBytes = (rowBytes * height) + ((rowBytes * height + 1) >> 1);
    numRows = height + ((height + 1) >> 1);
  } else {
    uint32_t bpp = BitsPerPixel(fmt);
    rowBytes = (width * bpp + 7) / 8; // round up to nearest byte
    numRows = height;
    numBytes = rowBytes * height;
  }

  if (outNumBytes)
    *outNumBytes = numBytes;
  if (outRowBytes)
    *outRowBytes = rowBytes;
  if (outNumRows)
    *outNumRows = numRows;
}
}


ImageDataPtr LoadDDSImageFromMemory(const uint8_t* contents, int32_t contents_len) {
  const DDS_HEADER* head = (const DDS_HEADER*)(contents + sizeof(uint32_t));
  // int32_t width = head->width;
  int32_t arraysize, depth, height;
  uint32_t format;
  int32_t tex_type = GetDDSDetail(*head, &height, &depth, &arraysize, &format);
  if (tex_type == kUnkonwnTexType) {
    return ImageDataPtr();
  }

  int32_t  mipcount = head->mipMapCount == 0 ? 1 : head->mipMapCount;
  bool bDXT10Header = false;
  if ((head->ddspf.flags & DDS_FOURCC) &&
      (MAKEFOURCC('D', 'X', '1', '0') == head->ddspf.fourCC)) {
    if ((sizeof(uint32_t) + sizeof(DDS_HEADER) + sizeof(DDS_HEADER_DXT10))
        < contents_len) {
      return ImageDataPtr();
    }
  }

  ImageDataPtr data(new ImageData(tex_type));
  ptrdiff_t offset = sizeof(uint32_t) + sizeof(DDS_HEADER)
      + (bDXT10Header ? sizeof(DDS_HEADER_DXT10) : 0);
  uint8_t* ptr = (uint8_t*)(contents + offset);
  for (int32_t i = 0; i < arraysize; ++i) {
    int32_t w = head->width;
    int32_t h = height;
    int32_t d = depth;
    // int32_t row_width = 0;
    for (int32_t mip = 0; mip < mipcount; ++mip) {
      uint32_t bytes, row_bytes, rows_num;
      GetSurfaceInfo(w, h, format, &bytes, &row_bytes, &rows_num);
      ImageLevelDataPtr leveldata(new ImageLevelData(
          w, h, d, ptr, bytes,  row_bytes, detail::TranslateFormat(format)));
      data->AppendData(leveldata.get());
      ptr += bytes * d;
      w = w >> 1;
      h = h >> 1;
      d = d >> 1;
      if (w == 0) w = 1;
      if (h == 0) h = 1;
      if (d == 0) d = 1;
    }
  }
  
  return data;
}

ImageDataPtr LoadDDSImage(const base::FilePath& path) {
  std::string contents;
  if (::base::ReadFileToString(path, &contents)) {
    return LoadDDSImageFromMemory(
        (const uint8_t*)contents.c_str(), static_cast<int32_t>(contents.length()));
  } else {
    return ImageDataPtr();
  }
}
}  // namespace azer
