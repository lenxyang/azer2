#include "azer/base/image.h"

#include <cmath>
#include <algorithm>
#include "base/files/file_path.h"

namespace azer {

uint32 ImageLevelData::pixel_size(int32 format) {
  return SizeofDataFormat((DataFormat)format);
}

ImageLevelData::ImageLevelData(int32 width, int32 height, int32 depth, 
                               uint8* data, int32 data_size, int32 row_bytes,
                               int32 format) 
    : width_(width),
      height_(height),
      depth_(depth),
      row_bytes_(row_bytes),
      data_format_(format),
      data_size_(data_size) {
  DCHECK(valid_params());
  data_.reset(new uint8[data_size_]);
  memcpy(data_.get(), data, data_size_);
}

bool ImageLevelData::valid_params() {
  return true;
}

int32 ImageLevelData::dim_data_size() const {
  return row_bytes() * height();
}

const uint8* ImageLevelData::dim_data(int32 depth) const {
  return data() + depth * dim_data_size();
}

// class ImageData
void ImageData::AppendData(ImageLevelData* data) {
  levels_.push_back(data);
}

int32 ImageData::width() const {
  return levels_[0]->width();
}

int32 ImageData::height() const {
  return levels_[0]->height();
}

int32 ImageData::depth() const {
  return levels_[0]->depth();
}

int32 ImageData::data_format() const {
  return levels_[0]->data_format();
}

const ImageLevelData* ImageData::GetLevelData(int32 level) const {
  DCHECK_LT(level, static_cast<int32>(levels_.size()));
  return levels_[level];
}
}  // namespace azer
