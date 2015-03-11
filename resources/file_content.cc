#include "azer/resources/file_content.h"

#include "base/logging.h"

namespace azer {
namespace resources {
FileContent::FileContent() 
    : length_(0)
    , capability_(0) {
}

FileContent::FileContent(int64 capability)
    : length_(0)
    , capability_(capability) {
  data_.reset(new uint8[this->capability()]);
}

FileContent::FileContent(const uint8* data, int64 size)
    : length_(size)
    , capability_(size) {
  data_.reset(new uint8[this->capability()]);
  memcpy(data_.get(), data, size);
}

FileContent::~FileContent() {
}
}  // namespace resources
}  // namespace azer
