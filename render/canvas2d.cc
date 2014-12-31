#include "azer/render/canvas2d.h"

#include "gl/GrGLFunctions.h"
#include "gl/GrGLDefines.h"
#include "gl/GrGLUtil.h"
#include "SkCanvas.h"
#include "SkGpuDevice.h"
#include "GrTexture.h"
#include "GrContext.h"
#include "SkImageInfo.h"
#include "SkImageEncoder.h"

#include "azer/render/device2d.h"
#include "azer/render/context2d.h"
#include "base/strings/string_util.h"
#include "azer/render/texture.h"

namespace azer {

using ::base::FilePath;

// class Canvas2D
Canvas2D::Canvas2D(int width, int height, Context2D* ctx)
    : width_(width)
    , height_(height)
    , device_(NULL)
    , context_(ctx) {
}

Canvas2D::~Canvas2D() {
  if (device_) { delete device_;}
}

bool Canvas2D::Init() {
  device_ = new Device2D();
  return device_->Init(context_, this);
}

SkCanvas* Canvas2D::GetSkCanvas() {
  DCHECK(device_ != NULL);
  return device_->GetCanvas();
}

TexturePtr& Canvas2D::GetTexture() {
  if (!texture_.get()) {
    GrTexture* tex = device_->GetGrTex();
    InitTexture(tex->getTextureHandle());
  }
  return texture_;
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
}

bool Canvas2D::Save(const FilePath& path) {
  DCHECK(device_ != NULL);
  const FilePath::StringType ext = ::base::StringToLowerASCII(path.Extension());
  SkImageEncoder::Type type = ImageType(ext);
  std::string pathstr = ::base::WideToUTF8(path.value());
  SkBitmap bitmap;
  SkImageInfo info = SkImageInfo::Make(width(), height(), kRGBA_8888_SkColorType,
                                       kOpaque_SkAlphaType);
  bitmap.setInfo(info);
  bitmap.allocPixels();
  device_->GetCanvas()->readPixels(&bitmap, 0, 0);
  // return SkImageEncoder::EncodeFile(pathstr.c_str(), bitmap, type, 100);
  return false;
}

void Canvas2D::flush() {
}

}  // namespace azer
