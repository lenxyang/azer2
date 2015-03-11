#include "azer/resources/file_content.h"

#include "base/logging.h"

namespace azer {
namespace resources {
FileContent::FileContent() 
    : length_(0) {
}

FileContent::FileContent(const uint8* data, int32 size)
    : length_(size) {
  data_.reset(new uint8[size]);
  memcpy(data_.get(), data, size);
}

}  // namespace resources
}  // namespace azer
