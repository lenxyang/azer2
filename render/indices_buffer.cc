#include "azer/render/indices_buffer.h"

#include <limits>
#include "base/logging.h"
#include "azer/render/render_system.h"

namespace azer {

IndexType IndicesData::CalcFixType(int num) {
  // directX donnot support kUint8
  /*if (num < std::numeric_limits<uint8>::max()) {
    return kUint8;
    } else */
  if (num < std::numeric_limits<uint16>::max()) {
    return kIndexUint16;
  } else if (num < std::numeric_limits<uint32>::max()) {
    return kIndexUint32;
  } else {
    NOTIMPLEMENTED();
    return kIndexUint32;
  }
}

IndicesData::IndicesData(int num)
    : Resource(kIndicesData)
    , type_(CalcFixType(num)), size_(0) {
  size_ = num * unit_size();
  num_ = num;
  data_.reset(new uint8[size_]);
}

IndicesData::IndicesData(int num, IndexType type)
    : Resource(kIndicesData)
    , type_(type), size_(0) {
  size_ = num * unit_size();
  num_ = num;
  data_.reset(new uint8[size_]);
}

int32 IndicesData::unit_size() const {
  switch (type()) {
    case kIndexUint8: return (int32)sizeof(uint8);
    case kIndexUint16: return (int32)sizeof(uint16);
    case kIndexUint32: return (int32)sizeof(uint32);
    default: CHECK(false); return 0;
  }
}

int32 IndicesData::index_value(int idx) const {
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

IndicesBuffer::IndicesBuffer(const Options& opt)
    : options_(opt)
    , indices_num_(-1)
    , type_(kIndexUndefined) {
}

IndicesBuffer::~IndicesBuffer() {
}

IndicesBuffer::Options::Options()
    : usage(GraphicBuffer::kDefault)
    , cpu_access(kCPUNoAccess) {
}

IndicesBufferPtr IndicesBuffer::CreateDefaultIndicesBuffer(RenderSystem* rs,
                                                           IndicesData* data) {
  return rs->CreateIndicesBuffer(Options(), data);
}
}  // namespace azer
