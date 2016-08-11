#include "azer/render/indices_buffer.h"

#include <limits>
#include "base/logging.h"
#include "azer/render/render_system.h"

namespace azer {

IndexType IndicesData::CalcFixType(int c) {
  // directX donnot support kUint8
  uint32_t count = static_cast<uint32_t>(c);
  /*if (count < std::counteric_limits<uint8>::max()) {
    return kUint8;
    } else */
  if (count < std::numeric_limits<uint16_t>::max()) {
    return kIndexUint16;
  } else if (count < std::numeric_limits<uint32_t>::max()) {
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
  data_.reset(new uint8_t[size]);
}

IndicesData::IndicesData(int count, IndexType type)
    : type_(type) {
  int32_t size = count * unit_size();
  count_ = count;
  data_.reset(new uint8_t[size]);
}

void IndicesData::extend(int32_t count) {
  int32_t size = (count_ + count) * unit_size();
  if (CalcFixType(count_ + count) == type()) {
    uint8_t* ndata = new uint8_t[size];
    memcpy(ndata, data_.get(), count_ * unit_size());
    data_.reset(ndata);
  } else {
    CHECK(false);
  }
  count_ += count;
}

int32_t IndicesData::unit_size() const {
  switch (type()) {
    case kIndexUint8: return (int32_t)sizeof(uint8_t);
    case kIndexUint16: return (int32_t)sizeof(uint16_t);
    case kIndexUint32: return (int32_t)sizeof(uint32_t);
    default: CHECK(false); return 0;
  }
}

int32_t IndicesData::index_value(int idx) const {
  switch (type()) {
    case kIndexUint8: 
      return *(uint8_t*)(data_.get() + idx);
    case kIndexUint16: 
      return *(int16_t*)(data_.get() + idx * sizeof(int16_t));
    case kIndexUint32: 
      return *(int32_t*)(data_.get() + idx * sizeof(uint32_t));
    default: CHECK(false); return 0;
  }
}

const uint8_t* IndicesData::pointer() const {
  DCHECK(data_.get() != NULL);
  return data_.get();
}

uint8_t* IndicesData::pointer() {
  DCHECK(data_.get() != NULL);
  return data_.get();
}

// class IndicesBuffer
IndicesBuffer::IndicesBuffer(const IndicesData* data)
    : indices_count_(data->count()),
      type_(data->type()) {
  RenderSystem* rs = RenderSystem::Current();
  CHECK(rs) << "RenderSystem Not Initialized";
  static GpuResOptions resopt;
  resopt.target = kBindTargetIndicesBuffer;
  gpu_buffer_ = rs->CreateBufferWithData(resopt, 1, data->size(), data->pointer());
  CHECK(gpu_buffer_.get()) << "Initializer GpuBuffer for ShaderParam Failed";
}

IndicesBuffer::IndicesBuffer(const GpuResOptions& opt, const IndicesData* data)
    : indices_count_(data->count()),
      type_(data->type()) {
  RenderSystem* rs = RenderSystem::Current();
  CHECK(rs) << "RenderSystem Not Initialized";
  CHECK(opt.target & kBindTargetIndicesBuffer);
  gpu_buffer_ = rs->CreateBufferWithData(opt, 1, data->size(), data->pointer());
  CHECK(gpu_buffer_.get()) << "Initializer GpuBuffer for ShaderParam Failed";
}

IndicesBuffer::~IndicesBuffer() {
}

}  // namespace azer
