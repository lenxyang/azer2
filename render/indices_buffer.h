#pragma once

#include <memory>


#include "azer/base/export.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/gpu_buffer.h"

namespace azer {
class IndicesData;
class IndicesBuffer;
typedef scoped_refptr<IndicesData> IndicesDataPtr;
typedef scoped_refptr<IndicesBuffer> IndicesBufferPtr;

enum IndexType {
  kIndexUndefined = 0,
  kIndexUint8,
  kIndexUint16,
  kIndexUint32,
};
class AZER_EXPORT IndicesData : public ::base::RefCounted<IndicesData> {
 public:
  static IndexType CalcFixType(int count);

  explicit IndicesData(int count);
  IndicesData(int count, IndexType type);
  virtual ~IndicesData() {}

  void extend(int32_t count);
  int32_t size() const { return unit_size() * count();}
  const uint8_t* pointer() const;
  uint8_t* pointer();
  IndexType type() const { return type_;}
  int32_t count() const { return count_;}
  int32_t unit_size() const;
  int32_t index_value(int idx) const;
  int32_t index_value(uint8_t* ptr) const;

  void reset() { data_.reset();}
 protected:
  IndexType type_;
  int32_t count_;
  std::unique_ptr<uint8_t[]> data_;
  DISALLOW_COPY_AND_ASSIGN(IndicesData);
};

class AZER_EXPORT IndicesBuffer : public GpuBuffer {
 public:
  IndicesBuffer(const GpuBufferOptions& opt);

  virtual ~IndicesBuffer();

  virtual GpuBufferDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;
  IndexType type() const { return type_;}
  int32_t indices_count() const { return indices_count_;}
 protected:
  int32_t indices_count_;
  IndexType type_;
  DISALLOW_COPY_AND_ASSIGN(IndicesBuffer);
};

}  // namespace azer
