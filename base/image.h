#pragma once

#include <memory>
#include <vector>

#include "base/memory/ref_counted.h"
#include "azer/base/string.h"
#include "azer/render/common.h"

namespace azer {
class ImageData;
class ImageLevelData;
typedef scoped_refptr<ImageData> ImageDataPtr;
typedef scoped_refptr<ImageLevelData> ImageLevelDataPtr;

class AZER_EXPORT ImageLevelData : public ::base::RefCounted<ImageLevelData> {
 public:
  ImageLevelData(int32_t width, int32_t height, int32_t depth, uint8_t* data, 
                 int32_t data_size, int32_t row_bytes, int32_t format);

  const uint8_t* data() const { return data_.get();}
  int32_t data_size() const { return data_size_;}
  const uint8_t* dim_data(int32_t depth) const;
  int32_t dim_data_size() const;
  
  int32_t pixel_size() const { return pixel_size(data_format());}
  int32_t width() const { return width_;}
  int32_t height() const { return height_;}
  int32_t depth() const { return depth_;}
  int32_t row_bytes() const { return row_bytes_;}
  int32_t data_format() const { return data_format_;}
  
  static uint32_t pixel_size(int32_t format); 
 private:
  bool valid_params();
  int32_t width_;
  int32_t height_;
  int32_t depth_;
  int32_t row_bytes_;
  int32_t data_format_;
  int32_t data_size_;
  std::unique_ptr<uint8_t> data_;
  DISALLOW_COPY_AND_ASSIGN(ImageLevelData);
};

class AZER_EXPORT ImageData : public ::base::RefCounted<ImageData> {
 public:
  explicit ImageData(int textype) : textype_(textype) {}

  int32_t width() const;
  int32_t height() const;
  int32_t depth() const;

  int32_t textype() const { return textype_;}
  int32_t data_format() const;
  void AppendData(ImageLevelData* data);
  int32_t level_count() const { return static_cast<int32_t>(levels_.size());}
  const ImageLevelData* GetLevelData(int32_t level) const;
 private:
  int32_t textype_;
  std::vector<ImageLevelDataPtr> levels_;
  DISALLOW_COPY_AND_ASSIGN(ImageData);
};

}  // namespace azer
