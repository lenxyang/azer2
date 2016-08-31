#include "azer/render/index_pack.h"

#include <limits>
#include "base/logging.h"
#include "azer/render/entity.h"

namespace azer {
IndexPack::IndexPack(IndicesData* data) 
    : idata_(data),
      offset_(0) {
}

IndexPack::IndexPack(EntityData* data) 
    : idata_(data->idata()),
      offset_(0) {
  CHECK(data && data->idata());
}

void IndexPack::move(int step) {
  offset_ = step * step_size();
  DCHECK(current() < pointer() + idata_->size());
}

bool IndexPack::advance(int step) const {
  offset_ += step * step_size();
  return current() < pointer() + idata_->size();
}

int32_t IndexPack::index() const {
  return offset_ / step_size();
}

void IndexPack::write(int value) {
  switch (type()) {
    case kIndexUint16:
      DCHECK(value < std::numeric_limits<uint16_t>::max());
      *(uint16_t*)(current()) = value;
      break;
    case kIndexUint32:
      *(uint32_t*)(current()) = value;
      break;
    case kIndexUint8:
      DCHECK(value < std::numeric_limits<uint8_t>::max());
      *(uint8_t*)(current()) = value;
      break;
    default:
      NOTREACHED();
  }
}

uint32_t IndexPack::value(int index) const {
  const uint8_t* ptr = pointer() + (step_size() * index);
  uint32_t value;
  switch (type()) {
    case kIndexUint16:
      value = *(uint16_t*)(ptr);
      break;
    case kIndexUint32:
      value = *(uint32_t*)(ptr);
      break;
    case kIndexUint8:
      value = *(uint8_t*)(ptr);
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
      value = *(uint16_t*)(current());
      break;
    case kIndexUint32:
      value = *(uint32_t*)(current());
      break;
    case kIndexUint8:
      value = *(uint8_t*)(current());
      break;
    default:
      NOTREACHED();
      value = 0;
  }
  return value;
}

bool IndexPack::WriteAndAdvance(int value) {
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

bool IndexPack::ReadAndAdvance(uint32_t* value) const {
  if (current() - pointer() < idata_->size()) {
    *value = this->value();
    advance();
    return true;
  } else {
    return false;
  }
}
}  // namespace azer
