#pragma once

#include <memory>
#include <vector>

#include "azer/base/image_data.h"
#include "azer/base/string.h"
#include "azer/render/texture.h"

namespace azer {

class AZER_EXPORT Image : public base::RefCounted<Image> {
 public:
  enum Type {
    k1D = kTex1D,
    k2D = kTex2D,
    k3D = kTex3D,
    kCubemap = kTexCubemap,
  };

  Image(ImageDataPtr& image, Type type);
  Image(ImageDataPtrVec& image, Type type);
  Image(const ImageDataPtrVec& image, Type type);

  int32 width() const;
  int32 height() const;
  int32 depth() const;
  Type type() const { return type_;}

  DataFormat format() const;

  ImageDataPtr& data(int index);
  const ImageDataPtr& data(int index) const;

  static Image* Load(const ::base::FilePath& path, Type type);
  static Image* Load(const StringType& path, Type type);
  static Image* Load(const char* data, int length, Type type);
 private:
  ImageDataPtrVec data_;
  Type type_;
  DISALLOW_COPY_AND_ASSIGN(Image);
};

typedef scoped_refptr<Image> ImagePtr;

inline Image::Image(ImageDataPtr& image, Type type)
    : type_(type) {
  DCHECK(type == k2D || type == k1D);
  data_.push_back(image);
}

inline Image::Image(ImageDataPtrVec& image, Type type)
    : type_(type) {
  data_.swap(image);
}

inline Image::Image(const ImageDataPtrVec& image, Type type)
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
