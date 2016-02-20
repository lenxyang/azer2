#pragma once

#include <memory>
#include <vector>

#include "azer/base/image_data.h"
#include "azer/base/string.h"
#include "azer/render/texture.h"
#include "azer/render/common.h"

namespace azer {
class Image;
typedef scoped_refptr<Image> ImagePtr;

class AZER_EXPORT Image : public base::RefCounted<Image> {
 public:
  Image(ImageDataPtr& image, TexType type);
  Image(const ImageDataPtrVec& image, TexType type);

  int32 width() const;
  int32 height() const;
  int32 depth() const;
  TexType type() const { return type_;}

  DataFormat format() const;

  ImageDataPtr& data(int index);
  const ImageDataPtr& data(int index) const;

  static ImagePtr LoadFromFile(const ::base::FilePath& path, TexType type);
  static ImagePtr LoadFromFile(const StringType& path, TexType type);
  static ImagePtr LoadFromFile(const std::vector<::base::FilePath>& path,
                             TexType type);
  static ImagePtr LoadFromMemory(const char* data, int length, TexType type);
 private:
  ImageDataPtrVec data_;
  TexType type_;
  DISALLOW_COPY_AND_ASSIGN(Image);
};

inline Image::Image(ImageDataPtr& image, TexType type)
    : type_(type) {
  DCHECK(type == kTex2D || type == kTex1D);
  data_.push_back(image);
}

inline Image::Image(const ImageDataPtrVec& image, TexType type)
    : type_(type) {
  data_ = image;
}

inline int32 Image::width() const {
  DCHECK_GT(data_.size(), 0u);
  return data_[0]->width();
}

inline int32 Image::height() const {
  DCHECK_GT(data_.size(), 0u);
  return data_[0]->height();
}

inline int32 Image::depth() const {
  DCHECK_GT(data_.size(), 0u);
  return static_cast<int32>(data_.size());
}

inline DataFormat Image::format() const {
  DCHECK_GT(data_.size(), 0u);
  return data_[0]->format();
}

inline ImageDataPtr& Image::data(int index) {
  DCHECK_GT(data_.size(), 0u);
  DCHECK_LT(index, data_.size());
  return data_[index];
}

inline const ImageDataPtr& Image::data(int index) const {
  DCHECK_GT(data_.size(), 0u);
  DCHECK_LT(index, data_.size());
  return data_[index];
}

}  // namespace azer
