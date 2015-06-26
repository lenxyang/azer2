#include "azer/render/indices_buffer.h"

namespace azer {
IndicesData::IndicesData(int num, IndexType type)
    : Resource(kIndicesData)
    , type_(type), size_(0) {
  size_ = num * unit_size();
  num_ = num;
  data_.reset(new uint8[size_]);
}

int32 IndicesData::unit_size() const {
  switch (type()) {
    case kUint8: return (int32)sizeof(uint8);
    case kUint16: return (int32)sizeof(uint16);
    case kUint32: return (int32)sizeof(uint32);
    default: CHECK(false); return 0;
  }
}

int32 IndicesData::index_value(int idx) const {
  switch (type()) {
    case kUint8: 
      return *(uint8*)(data_.get() + idx);
    case kUint16: 
      return *(int16*)(data_.get() + idx * sizeof(int16));
    case kUint32: 
      return *(int32*)(data_.get() + idx * sizeof(uint32));
    default: CHECK(false); return 0;
  }
}

IndicesBuffer::IndicesBuffer(const Options& opt)
    : options_(opt)
    , indices_num_(-1)
    , type_(IndicesData::kUndefined) {
}

IndicesBuffer::~IndicesBuffer() {
}

IndicesBuffer::Options::Options()
    : usage(GraphicBuffer::kDefault)
    , cpu_access(kCPUNoAccess) {
}
}  // namespace azer
