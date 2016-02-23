#pragma once

#include <memory>
#include <vector>

#include "azer/base/string.h"
#include "azer/render/texture.h"
#include "azer/render/common.h"

namespace azer {
class ImageData;
class ImageLevelData;
typedef scoped_refptr<ImageData> ImageDataPtr;
typedef scoped_refptr<ImageLevelData> ImageLevelDataPtr;

class AZER_EXPORT ImageLevelData : public ::base::RefCounted<ImageLevelData> {
 public:
  ImageLevelData(int32 width, int32 height, int32 depth, uint8* data, 
                 int32 data_size, int32 row_bytes, int32 format);

  const uint8* data() const { return data_.get();}
  int32 data_size() const { return data_size_;}
  const uint8* dim_data(int32 depth) const;
  int32 dim_data_size() const;
  
  int32 pixel_size() const { return pixel_size(data_format());}
  int32 width() const { return width_;}
  int32 height() const { return height_;}
  int32 depth() const { return depth_;}
  int32 row_bytes() const { return row_bytes_;}
  int32 data_format() const { return data_format_;}
  
  static uint32 pixel_size(int32 format); 
 private:
  bool valid_params();
  int32 width_;
  int32 height_;
  int32 depth_;
  int32 row_bytes_;
  int32 data_format_;
  int32 data_size_;
  scoped_ptr<uint8> data_;
  DISALLOW_COPY_AND_ASSIGN(ImageLevelData);
};

class AZER_EXPORT ImageData : public ::base::RefCounted<ImageData> {
 public:
  explicit ImageData(int textype) : textype_(textype) {}

  int32 width() const;
  int32 height() const;
  int32 depth() const;

  int32 textype() const { return textype_;}
  int32 data_format() const;
  void AppendData(ImageLevelData* data);
  int32 level_count() const { return static_cast<int32>(levels_.size());}
  const ImageLevelData* GetLevelData(int32 level) const;
 private:
  int32 textype_;
  std::vector<ImageLevelDataPtr> levels_;
  DISALLOW_COPY_AND_ASSIGN(ImageData);
};

}  // namespace azer
