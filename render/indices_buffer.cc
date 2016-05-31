#include "azer/render/indices_buffer.h"

#include <limits>
#include "base/logging.h"
#include "azer/render/render_system.h"

namespace azer {

IndexType IndicesData::CalcFixType(int count) {
  // directX donnot support kUint8
  /*if (count < std::counteric_limits<uint8>::max()) {
    return kUint8;
    } else */
  if (count < std::numeric_limits<uint16>::max()) {
    return kIndexUint16;
  } else if (count < std::numeric_limits<uint32>::max()) {
    return kIndexUint32;
  } else {
    NOTIMPLEMENTED();
    return kIndexUint32;
  }
}

IndicesData::IndicesData(int count)
    : type_(CalcFixType(count)) {
  int32_t size = count * unit_size();
  count_ = count;
  data_.reset(new uint8[size]);
}

IndicesData::IndicesData(int count, IndexType type)
    : type_(type) {
  int32_t size = count * unit_size();
  count_ = count;
  data_.reset(new uint8[size]);
}

void IndicesData::extend(int32_t count) {
  int32_t size = (count_ + count) * unit_size();
  if (CalcFixType(count_ + count) == type()) {
    uint8* ndata = new uint8[size];
    memcpy(ndata, data_.get(), count_ * unit_size());
    data_.reset(ndata);
  } else {
    CHECK(false);
  }
  count_ += count;
}

int32_t IndicesData::unit_size() const {
  switch (type()) {
    case kIndexUint8: return (int32)sizeof(uint8);
    case kIndexUint16: return (int32)sizeof(uint16);
    case kIndexUint32: return (int32)sizeof(uint32);
    default: CHECK(false); return 0;
  }
}

int32_t IndicesData::index_value(int idx) const {
  switch (type()) {
    case kIndexUint8: 
      return *(uint8*)(data_.get() + idx);
    case kIndexUint16: 
      return *(int16*)(data_.get() + idx * sizeof(int16));
    case kIndexUint32: 
      return *(int32*)(data_.get() + idx * sizeof(uint32));
    default: CHECK(false); return 0;
  }
}

const uint8* IndicesData::pointer() const {
  DCHECK(data_.get() != NULL);
  return data_.get();
}

uint8* IndicesData::pointer() {
  DCHECK(data_.get() != NULL);
  return data_.get();
}

// class IndicesBuffer
IndicesBuffer::IndicesBuffer(const HBufferOptions& opt)
    : HardwareBuffer(opt)
    , indices_count_(-1)
    , type_(kIndexUndefined) {
}

IndicesBuffer::~IndicesBuffer() {
}

}  // namespace azer
