#pragma once

#include <memory>
#include <vector>

#include "azer/base/image_data.h"
#include "azer/render/texture.h"

namespace azer {

class AZER_EXPORT Image {
 public:
  enum Type {
    k1D = Texture::k1D,
    k2D = Texture::k2D,
    k3D = Texture::k3D,
    kCubeMap = Texture::kCubeMap,
  };
  explicit Image(std::vector<ImageDataPtr>& image, Type type)
      : type_(type) {
    data_.swap(image);
  }

  explicit Image(const std::vector<ImageDataPtr>& image, Type type)
      : type_(type) {
    data_ = image;
  }

  int32 width() const;
  int32 height() const;
  int32 depth() const;
  Type type() const { return type_;}

  DataFormat format() const;

  ImageDataPtr& data(int index);
  const ImageDataPtr& data(int index) const;

  static Image* Load(const ::base::FilePath& path);
  static Image* Load(const ::base::FilePath::StringType& path);
 private:
  std::vector<ImageDataPtr> data_;
  Type type_;
  DISALLOW_COPY_AND_ASSIGN(Image);
};

typedef std::shared_ptr<Image> ImagePtr;

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

inline Image* Image::Load(const ::base::FilePath::StringType& path) {
  return Load(::base::FilePath(path));
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
