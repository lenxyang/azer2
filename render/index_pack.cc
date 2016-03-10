#include "azer/render/index_pack.h"

#include <limits>
#include "base/logging.h"

namespace azer {
IndexPack::IndexPack(uint8* data, int32 data_size, IndexType type)
    : data_(data),
      current_(data_),
      data_size_(data_size),
      type_(type) {
}

IndexPack::IndexPack(IndicesData* data) 
    : data_(data->pointer()), 
      current_(data_),
      data_size_(data->size()),
      type_(data->type()) {
}

int32 IndexPack::step_size() const {
  uint32 unit_size = 0;
  switch (type()) {
    case kIndexUint16:
      return sizeof(uint16);
    case kIndexUint32:
      return sizeof(uint32);
    case kIndexUint8:
      return sizeof(uint8);
    default:
      NOTREACHED();
      return 0;
  }
}

void IndexPack::move(int32 step) {
  current_ = data_ + (step_size() * step);
  DCHECK(current_ < data_ + data_size_);
}

bool IndexPack::advance(int32 step) const {
  current_ += (step_size() * step);
  return current_ < data_ + data_size_;
}

void IndexPack::write(int32 value) {
  switch (type()) {
    case kIndexUint16:
      DCHECK(value < std::numeric_limits<uint16>::max());
      *(uint16*)(current_) = value;
      break;
    case kIndexUint32:
      *(uint32*)(current_) = value;
      break;
    case kIndexUint8:
      DCHECK(value < std::numeric_limits<uint8>::max());
      *(uint8*)(current_) = value;
      break;
    default:
      NOTREACHED();
  }
}

uint32 IndexPack::value(int32 index) const {
  uint8* ptr = data_ + (step_size() * index);
  uint32 value;
  switch (type()) {
    case kIndexUint16:
      value = *(uint16*)(ptr);
      break;
    case kIndexUint32:
      value = *(uint32*)(ptr);
      break;
    case kIndexUint8:
      value = *(uint8*)(ptr);
      break;
    default:
      NOTREACHED();
      value = 0;
  }
  return value;
}

uint32 IndexPack::value() const {
  CHECK(current_ < data_ + data_size_);
  uint32 value;
  switch (type()) {
    case kIndexUint16:
      value = *(uint16*)(current_);
      break;
    case kIndexUint32:
      value = *(uint32*)(current_);
      break;
    case kIndexUint8:
      value = *(uint8*)(current_);
      break;
    default:
      NOTREACHED();
      value = 0;
  }
  return value;
}

bool IndexPack::WriteAndAdvance(int32 value) {
  if (current_ - data_ < data_size_) {
    write(value);
    advance();
    return true;
  } else {
    return false;
  }
}

uint32 IndexPack::ReadAndAdvanceOrDie() const {
  DCHECK(current_ - data_ < data_size_);
  uint32 value = this->value();
  advance();
  return value;
}

bool IndexPack::ReadAndAdvance(uint32* value) const {
  if (current_ - data_ < data_size_) {
    *value = this->value();
    advance();
    return true;
  } else {
    return false;
  }
}

int32 IndexPack::count() const {
  switch (type()) {
    case kIndexUint16:
      return data_size_ / sizeof(uint16);
    case kIndexUint32:
      return data_size_ / sizeof(uint32);
    case kIndexUint8:
      return data_size_ / sizeof(uint8);
    default:
      NOTREACHED();
      return 0;
  }
}
}  // namespace azer
