#pragma once

#include <memory>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "azer/math/math.h"
#include "azer/base/export.h"
#include "azer/render/common.h"

namespace azer {

class ImageData;
typedef scoped_refptr<ImageData> ImageDataPtr;
typedef std::vector<ImageDataPtr> ImageDataPtrVec;

class AZER_EXPORT ImageData : public ::base::RefCounted<ImageData> {
 public:
  ImageData(int width, int height, DataFormat format = kRGBAn8);

  int width() const;
  int height() const;
  uint8* data();
  const uint8* data() const;

  uint32 pixel(int x, int y) const;
  void set_pixel(int x, int y, uint32 v);
  int32 data_size() const;
  int32 unit_size() const { return sizeof_dataformat(format_);}
  DataFormat format() const { return format_;}

  // sample functions
  // tu and tv [0, 1)
  Vector4 BoxSample(float tu, float tv) const;

  static ImageDataPtr Load2D(const char* data, int32 length);
  static ImageDataPtr Load2DFromFile(const ::base::FilePath& path);
  static ImageDataPtrVec LoadCubemap(const char* data, int32 length);
  static ImageDataPtrVec LoadCubemapFromFile(const ::base::FilePath& path);
 private:
  int32 sizeof_dataformat(DataFormat format) const;
  std::unique_ptr<uint8> data_;
  const int32 width_;
  const int32 height_;
  const DataFormat format_;
  DISALLOW_COPY_AND_ASSIGN(ImageData);
};

}  // namespace azer
