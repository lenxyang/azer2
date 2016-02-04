#pragma once

#include <memory>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/common.h"

namespace azer {

class Renderer;
class HardwareBufferData;
class HardwareBuffer;
typedef scoped_refptr<HardwareBufferData> HardwareBufferDataPtr;

class AZER_EXPORT HardwareBufferData: public ::base::RefCounted<HardwareBufferData> {
 public:
  HardwareBufferData();
  uint8* data_ptr() const { return data_;}
  int32 row_size() const { return row_size_;}
  int32 column_num() const { return column_num_;}
 private:
  uint8* data_;
  int32 row_size_;
  int32 column_num_;
  int32 size_;

  friend class HardwareBuffer;
  DISALLOW_COPY_AND_ASSIGN(HardwareBufferData);
};

struct AZER_EXPORT HBufferOptions {
  char name[128];
  BufferUsage usage;
  CPUAccess cpu_access;  // defined render_system
  uint32 target;
  HBufferOptions();
};

class AZER_EXPORT HardwareBuffer : public ::base::RefCounted<HardwareBuffer> {
 public:
  HardwareBuffer(const HBufferOptions& opt) {}
  virtual ~HardwareBuffer() {}

  virtual HardwareBufferDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;
 protected:
  void SetLockDataPtr(void* ptr, HardwareBufferData* data) {
    data->data_ = (uint8*)ptr;
  }
  void SetLockDataRowSize(int32 size, HardwareBufferData* data) {
    data->row_size_ = size;
  }
  void SetLockDataColumnNum(int32 num, HardwareBufferData* data) {
    data->column_num_ = num;
  }

  const HBufferOptions& options() const { return options_;}
 protected:
  const HBufferOptions options_;
  DISALLOW_COPY_AND_ASSIGN(HardwareBuffer);
};

class AZER_EXPORT AutoBufferLock {
 public:
  AutoBufferLock(HardwareBuffer* hbuffer, MapType flags, Renderer* renderer)
      : hbuffer_(hbuffer) {
    data_ = hbuffer_->map(flags);
  }
  ~AutoBufferLock() {
    if (data_.get()) {
      hbuffer_->unmap();
    }
  }
 private:
  HardwareBufferDataPtr data_;
  HardwareBuffer* hbuffer_;
  DISALLOW_COPY_AND_ASSIGN(AutoBufferLock);
};

AZER_EXPORT const HBufferOptions& kVertexBufferOpt();
AZER_EXPORT const HBufferOptions& kIndicesBufferOpt();
AZER_EXPORT const HBufferOptions& kShaderConstsTableBufferOpt();
}  // namespace azer
