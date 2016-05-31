#include "azer/render/index_pack.h"

#include <limits>
#include "base/logging.h"

namespace azer {
IndexPack::IndexPack(IndicesData* data) 
    : idata_(data),
      offset_(0) {
}

void IndexPack::move(int32_t step) {
  offset_ = step * step_size();
  DCHECK(current() < pointer() + idata_->size());
}

bool IndexPack::advance(int32_t step) const {
  offset_ += step * step_size();
  return current() < pointer() + idata_->size();
}

int32_t IndexPack::index() const {
  return offset_ / step_size();
}

void IndexPack::write(int32_t value) {
  switch (type()) {
    case kIndexUint16:
      DCHECK(value < std::numeric_limits<uint16>::max());
      *(uint16*)(current()) = value;
      break;
    case kIndexUint32:
      *(uint32*)(current()) = value;
      break;
    case kIndexUint8:
      DCHECK(value < std::numeric_limits<uint8>::max());
      *(uint8*)(current()) = value;
      break;
    default:
      NOTREACHED();
  }
}

uint32_t IndexPack::value(int32_t index) const {
  const uint8* ptr = pointer() + (step_size() * index);
  uint32_t value;
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

uint32_t IndexPack::value() const {
  CHECK(current() < pointer() + idata_->size());
  uint32_t value;
  switch (type()) {
    case kIndexUint16:
      value = *(uint16*)(current());
      break;
    case kIndexUint32:
      value = *(uint32*)(current());
      break;
    case kIndexUint8:
      value = *(uint8*)(current());
      break;
    default:
      NOTREACHED();
      value = 0;
  }
  return value;
}

bool IndexPack::WriteAndAdvance(int32_t value) {
  if (current() - pointer() < idata_->size()) {
    write(value);
    advance();
    return true;
  } else {
    return false;
  }
}

uint32_t IndexPack::ReadAndAdvanceOrDie() const {
  DCHECK(current() - pointer() < idata_->size());
  uint32_t value = this->value();
  advance();
  return value;
}

bool IndexPack::ReadAndAdvance(uint32* value) const {
  if (current() - pointer() < idata_->size()) {
    *value = this->value();
    advance();
    return true;
  } else {
    return false;
  }
}
}  // namespace azer
