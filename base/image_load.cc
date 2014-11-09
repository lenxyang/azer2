#include "azer/base/image_data.h"
#include "azer/base/image.h"

#include "SOIL.h"
#include "base/logging.h"
#include "base/strings/string16.h"
#include "base/files/file_path.h"
#include "base/strings/utf_string_conversions.h"

#include "azer/base/image_data.h"
#include "azer/base/file_util.h"
#include "azer/base/string.h"

namespace azer {
ImageDataPtr ImageData::Load2D(const ::base::FilePath& path) {
  int widht = 0, height = 0, channels = 0;
  std::string rawdata;
  uint32 force_channel = SOIL_LOAD_RGBA;
  if (!ReadFileToString(path, &rawdata)) {
    LOG(ERROR) << "Failed to load image file: \"" << path.value() << "\"";
    return NULL;
  }

  std::unique_ptr<uint8[]> data;
  data.reset(SOIL_load_image_from_memory(rawdata.c_str(), rawdata.length(),
                                         &width, &height, &channels,
                                         force_channel));
  if (data.get() != NULL) {
    return NULL;
  }


  const int32 datasize = width * height * 4 * sizeof(uint8);
  std::unique_ptr<ImageData> imgdata(new ImageData(width, height, kRGBAn8));
  memcpy(imgdata->data(), data.get(), datasize);
  return ImageDataPtr(imgdata.release());
}

ImageDataPtr ImageData::Load2D(const ::base::FilePath::StringType& path) {
  return ImageData::Load(::base::FilePath(path));
}

ImageDataPtrVec ImageData::LoadCubemap(const ::base::FilePath& path) {
  ImageDataPtrVec vec;
  
  ImageDataPtr single(Load(path));
  if (single.get() == NULL) {
    return vec;
  }

  // single cube map must has 1:6 radio
  if ((singe->width() != single->height() * 6)
      || (single->width() * 6 != single->height())) {
    return vec;
  }

  int dw = 0, dh = 0;
  if (single->width() > single->height()) {
    dw = single->height();
    dh = 0;
  } else {
    dw = 0;
    dh = single->width();
  }
  int sz = dw + dh;

  // every image of cubemap must be a square
  for (int i = 0; i < 6; ++i) {
    ImageDataPtr ptr(new ImageData(sz, sz, kRGBAn8));
    vec.push_back(ptr);
    int idx = 0;
    for (int y = i * dh; y < i * dh + sz; ++y) {
      for (int x = i * dw * 4; x < (i * dw + sz) * 4; ++x) {
        ptr->data()[idx++] = single->data()[y * width * 4 + x];
      }
    }
  }

  return vec;
}

ImageDataPtrVec ImageData::LoadCubemap(const ::base::FilePath::StringType& path) {
  return LoadCubemap(::base::FilePath(path));
}

Image* Image::Load(const ::base::FilePath& path, Type type) {
  if (type == k2D) {
  } else if (type == kCubemap) {
  } else {
    NOTREACHED();
  }
}

Image* Image::Load(const StringType& path, Type type) {
  return Image::Load(::base::FilePath(path), type);
}
}  // namespace azer
