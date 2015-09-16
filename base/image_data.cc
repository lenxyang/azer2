#include "azer/base/image_data.h"

namespace azer {
ImageData::ImageData(int32 width, int32 height, DataFormat format)
    : width_(width)
    , height_(height)
    , format_(format) {
  uint32 size = data_size();
  data_.reset(new uint8[size]);
}

int ImageData::width() const {
  DCHECK(data_.get() != NULL);
  return width_;
}
int ImageData::height() const {
  DCHECK(data_.get() != NULL);
  return height_;
}

int32 ImageData::data_size() const {
  return width_ * height_ * sizeof_dataformat(format_);
}

uint8* ImageData::data() {
  DCHECK(data_.get() != NULL);
  return data_.get();
}

const uint8* ImageData::data() const {
  DCHECK(data_.get() != NULL);
  return data_.get();
}

int32 ImageData::sizeof_dataformat(DataFormat format) const {
  switch (format) {
    case kRGBA8:
    case kRGBAn8:
      return (int32)sizeof(uint32);
    case kRGBA32:
    case kRGBAn32:
      return (int32)sizeof(uint32) * 4;
    case kRGBAf:
      return (int32)sizeof(float) * 4;
    default:
      NOTREACHED();
      return -1;
  }
}

uint32 ImageData::pixel(int x, int y) const {
  DCHECK(data_.get() != NULL);
  uint8* ptr = data_.get() + (y * width() + x) * sizeof_dataformat(format());
  return *(uint32*)ptr;
}

void ImageData::set_pixel(int x, int y, uint32 v) {
  uint8* ptr = data_.get() + (y * width() + x) * sizeof_dataformat(format());
  *(uint32*)ptr = v;
}

}  // namespace azer
