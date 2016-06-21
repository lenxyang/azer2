#include "azer/base/image.h"

#include <cmath>
#include <algorithm>
#include "base/files/file_path.h"
#include "azer/render/texture.h"

namespace azer {

uint32_t ImageLevelData::pixel_size(TexFormat format) {
  return SizeofTexFormat(format);
}

ImageLevelData::ImageLevelData(int32_t width, int32_t height, int32_t depth, 
                               uint8_t* data, int32_t data_size, int32_t row_bytes,
                               TexFormat format) 
    : width_(width),
      height_(height),
      depth_(depth),
      row_bytes_(row_bytes),
      data_format_(format),
      data_size_(data_size) {
  DCHECK(valid_params());
  data_.reset(new uint8_t[data_size_]);
  memcpy(data_.get(), data, data_size_);
}

bool ImageLevelData::valid_params() {
  return true;
}

int32_t ImageLevelData::dim_data_size() const {
  return row_bytes() * height();
}

const uint8_t* ImageLevelData::dim_data(int32_t depth) const {
  return data() + depth * dim_data_size();
}

// class ImageData
void ImageData::AppendData(ImageLevelData* data) {
  levels_.push_back(data);
}

int32_t ImageData::width() const {
  return levels_[0]->width();
}

int32_t ImageData::height() const {
  return levels_[0]->height();
}

int32_t ImageData::depth() const {
  return levels_[0]->depth();
}

TexFormat ImageData::data_format() const {
  return levels_[0]->data_format();
}

const ImageLevelData* ImageData::GetLevelData(int32_t level) const {
  DCHECK_LT(level, static_cast<int32_t>(levels_.size()));
  return levels_[level].get();
}
}  // namespace azer
