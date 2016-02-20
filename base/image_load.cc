#include "azer/base/image_data.h"
#include "azer/base/image.h"

#include "SOIL.h"
#include "base/logging.h"
#include "base/strings/string16.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/strings/utf_string_conversions.h"

#include "azer/base/image_data.h"
#include "azer/base/file_util.h"
#include "azer/base/string.h"

namespace azer {
ImageDataPtr ImageData::Load2DFromFile(const ::base::FilePath& path) {
  std::string contents;
  if (!base::ReadFileToString(path, &contents)) {
    LOG(ERROR) << "Failed to read contents from \"" << path.value() << "\"";
    return ImageDataPtr();
  }

  return Load2D(contents.c_str(), contents.length());
}
  
ImageDataPtr ImageData::Load2D(const char* rawdata, int32 length) {
  int width = 0, height = 0, channels = 0;
  uint32 force_channel = SOIL_LOAD_RGBA;
  std::unique_ptr<uint8[]> data;
  data.reset(SOIL_load_image_from_memory((const uint8*)rawdata, length,
                                         &width, &height, &channels,
                                         force_channel));
  if (data.get() == NULL) {
    return NULL;
  }

  const int32 datasize = width * height * 4 * sizeof(uint8);
  std::unique_ptr<ImageData> imgdata(new ImageData(width, height, kRGBAn8));
  memcpy(imgdata->data(), data.get(), datasize);
  return ImageDataPtr(imgdata.release());
}

ImageDataPtrVec ImageData::LoadCubemapFromFile(const ::base::FilePath& path)  {
  std::string contents;
  if (!base::ReadFileToString(path, &contents)) {
    LOG(ERROR) << "Failed to read contents from \"" << path.value() << "\"";
    return ImageDataPtrVec();
  }

  return LoadCubemap(contents.c_str(), contents.length());
}

ImageDataPtrVec ImageData::LoadCubemap(const char* data, int32 length) {
  ImageDataPtrVec vec;
  ImageDataPtr single(ImageData::Load2D(data, length));
  if (single.get() == NULL) {
    return vec;
  }

  // single cube map must has 1:6 radio
  if ((single->width() != single->height() * 6)
      && (single->width() * 6 != single->height())) {
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
        ptr->data()[idx++] = single->data()[y * single->width() * 4 + x];
      }
    }
  }

  return vec;
}

ImagePtr Image::LoadFromFile(const std::vector<::base::FilePath>& path, 
                             TexType type) {
  TexType item_type = kTex2D;
  if (type == kTex2DArray) {
    item_type = kTex2D;
  } else if (type == kTexCubemap) {
    DCHECK_EQ(path.size(), 6u);
    item_type = kTex2D;
  } else {
    NOTREACHED();
  }

  ImageDataPtrVec vec;
  for (auto iter = path.begin(); iter != path.end(); ++iter) {
    ImageDataPtr ptr;
    if (item_type == kTex2D) {
      ptr = ImageData::Load2DFromFile(*iter);
    } else {
      NOTREACHED();
    }
    if (ptr.get()) {
      vec.push_back(ptr);
    } else {
      return ImagePtr();
    }
  }

  return ImagePtr(new Image(vec, type));
}

ImagePtr Image::LoadFromMemory(const char* data, int length, TexType type) {
  if (type == kTex2D) {
    ImageDataPtr ptr(ImageData::Load2D(data, length));
    if (ptr.get()) {
      return ImagePtr(new Image(ptr, type));
    } else {
      return ImagePtr();
    }
  } else if (type == kTexCubemap) {
    ImageDataPtrVec vec = std::move(ImageData::LoadCubemap(data, length));
    if (vec.size() == 6u) {
      return ImagePtr(new Image(vec, type));
    } else {
      return ImagePtr();
    }
  } else {
    NOTREACHED();
    return ImagePtr();
  }
}

ImagePtr Image::LoadFromFile(const StringType& path, TexType type) {
  return Image::LoadFromFile(::base::FilePath(path), type);
}

ImagePtr Image::LoadFromFile(const ::base::FilePath& path, TexType type) {
  std::string rawdata;
  if (!::base::ReadFileToString(path, &rawdata)) {
    LOG(ERROR) << "Failed to load image file: \"" << path.value() << "\"";
    return NULL;
  }

  return LoadFromMemory(rawdata.c_str(), rawdata.length(), type);
}
}  // namespace azer
