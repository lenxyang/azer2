#pragma once

#include <memory>

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/base/resource.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/hardware_buffer.h"

namespace azer {
enum IndexType {
  kIndexUndefined = 0,
  kIndexUint8,
  kIndexUint16,
  kIndexUint32,
};
class AZER_EXPORT IndicesData : public Resource {
 public:
  static IndexType CalcFixType(int num);

  explicit IndicesData(int num);
  IndicesData(int num, IndexType type);
  ~IndicesData() override {}

  int32 size() const { return size_;}
  const uint8* pointer() const;
  uint8* pointer();
  IndexType type() const { return type_;}
  int32 num() const { return num_;}
  int32 unit_size() const;
  int32 index_value(int idx) const;
  int32 index_value(uint8* ptr) const;

  void reset() { data_.reset();}
 protected:
  IndexType type_;
  int32 size_;
  int32 num_;
  std::unique_ptr<uint8[]> data_;
  DISALLOW_COPY_AND_ASSIGN(IndicesData);
};

typedef scoped_refptr<IndicesData> IndicesDataPtr;

class IndicesBuffer;
typedef scoped_refptr<IndicesBuffer> IndicesBufferPtr;

class AZER_EXPORT IndicesBuffer : public HardwareBuffer {
public:
  struct AZER_EXPORT Options {
    GraphicBuffer::Usage usage;
    CPUAccess cpu_access;  // defined render_system
    Options();
  };

  static IndicesBufferPtr CreateDefaultIndicesBuffer(RenderSystem* rs,
                                                     IndicesData* data);

  IndicesBuffer(const Options& opt);

  virtual ~IndicesBuffer();

  virtual HardwareBufferDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;
  IndexType type() const { return type_;}

  const Options& options() const { return options_;}
  int32 indices_num() const { return indices_num_;}
 protected:
  const Options options_;
  int32 indices_num_;
  IndexType type_;
  DISALLOW_COPY_AND_ASSIGN(IndicesBuffer);
};

}  // namespace azer
