#pragma once

#include <memory>

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/hardware_buffer.h"

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

  void extend(int32 count);
  int32 size() const { return unit_size() * count();}
  const uint8* pointer() const;
  uint8* pointer();
  IndexType type() const { return type_;}
  int32 count() const { return count_;}
  int32 unit_size() const;
  int32 index_value(int idx) const;
  int32 index_value(uint8* ptr) const;

  void reset() { data_.reset();}
 protected:
  IndexType type_;
  int32 count_;
  std::unique_ptr<uint8[]> data_;
  DISALLOW_COPY_AND_ASSIGN(IndicesData);
};

class AZER_EXPORT IndicesBuffer : public HardwareBuffer {
 public:
  IndicesBuffer(const HBufferOptions& opt);

  virtual ~IndicesBuffer();

  virtual HardwareBufferDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;
  IndexType type() const { return type_;}
  int32 indices_count() const { return indices_count_;}
 protected:
  int32 indices_count_;
  IndexType type_;
  DISALLOW_COPY_AND_ASSIGN(IndicesBuffer);
};

}  // namespace azer
