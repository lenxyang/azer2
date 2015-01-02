#include "azer/render/canvas2d.h"


// skia headers
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkImageInfo.h"
#include "third_party/skia/include/core/SkImageEncoder.h"

#include "base/strings/string_util.h"
#include "base/files/file_util.h"

#include "ui/gfx/geometry/size.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/codec/png_codec.h"

#include "azer/render/texture.h"
#include "azer/render/context2d.h"

namespace azer {

using ::base::FilePath;

// class Canvas2D
Canvas2D::Canvas2D(int width, int height, Context2D* ctx)
    : width_(width)
    , height_(height)
    , skcanvas_(NULL)
    , context_(ctx) {
}

Canvas2D::~Canvas2D() {
}


namespace {
SkImageEncoder::Type ImageType(const FilePath::StringType& ext) {
  if (ext ==  FILE_PATH_LITERAL(".bmp")) {
    return SkImageEncoder::kBMP_Type;
  } else if (ext == FILE_PATH_LITERAL(".png")) {
    return SkImageEncoder::kPNG_Type;
  } else if (ext == FILE_PATH_LITERAL(".jpg") || ext == FILE_PATH_LITERAL(".jpeg")) {
    return SkImageEncoder::kJPEG_Type;
  } else if (ext == FILE_PATH_LITERAL(".gif")) {
    return SkImageEncoder::kGIF_Type;
  } else {
    NOTREACHED();
    return SkImageEncoder::kBMP_Type;
  }
}

std::vector<uint8> EncodeBitmap(const SkBitmap& bitmap, 
                                    SkImageEncoder::Type type) {
  std::vector<uint8> data;
  if (type == SkImageEncoder::kPNG_Type) {
    CHECK(gfx::PNGCodec::Encode((const uint8*)bitmap.getPixels(),
                                gfx::PNGCodec::FORMAT_BGRA,
                                gfx::Size(bitmap.width(), bitmap.height()),
                                bitmap.width() * 4,
                                true,
                                std::vector<gfx::PNGCodec::Comment>(),
                                &data));
    return data;
  } else {
    return data;
  }
}
}

bool Canvas2D::Save(const FilePath& path) {
  const FilePath::StringType ext = ::base::StringToLowerASCII(path.Extension());
  SkImageEncoder::Type type = ImageType(ext);
  std::string pathstr = ::base::WideToUTF8(path.value());
  SkBitmap bitmap;
  SkImageInfo info = SkImageInfo::Make(width(), height(), kRGBA_8888_SkColorType,
                                       kOpaque_SkAlphaType);
  bitmap.setInfo(info);
  bitmap.allocPixels();
  GetSkCanvas()->readPixels(&bitmap, 0, 0);
  std::vector<uint8> compressed = std::move(EncodeBitmap(bitmap, type));

  FILE* f = base::OpenFile(path, "wb");
  CHECK_EQ(fwrite(&*compressed.begin(), 1, compressed.size(), f),
           compressed.size());
  base::CloseFile(f);
  return true;
}

}  // namespace azer
