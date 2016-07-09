#pragma once

#include <memory>


#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/common.h"

namespace azer {

class Renderer;
class GpuBufferData;
class GpuBuffer;
typedef scoped_refptr<GpuBufferData> GpuBufferDataPtr;

class AZER_EXPORT GpuBufferData: public ::base::RefCounted<GpuBufferData> {
 public:
  GpuBufferData();
  uint8_t* data_ptr() const { return data_;}
  int32_t row_size() const { return row_size_;}
  int32_t column_num() const { return column_num_;}
 private:
  uint8_t* data_;
  int32_t row_size_;
  int32_t column_num_;
  int32_t size_;

  friend class GpuBuffer;
  DISALLOW_COPY_AND_ASSIGN(GpuBufferData);
};

struct AZER_EXPORT GpuBufferOptions {
  char name[128];
  BufferUsage usage;
  CPUAccess cpu_access;  // defined render_system
  uint32_t target;
  GpuBufferOptions();
};

class AZER_EXPORT GpuBuffer : public ::base::RefCounted<GpuBuffer> {
 public:
  GpuBuffer(const GpuBufferOptions& opt);
  virtual ~GpuBuffer();

  virtual GpuBufferDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;
 protected:
  void SetLockDataPtr(void* ptr, GpuBufferData* data) {
    data->data_ = (uint8_t*)ptr;
  }
  void SetLockDataRowSize(int32_t size, GpuBufferData* data) {
    data->row_size_ = size;
  }
  void SetLockDataColumnNum(int32_t num, GpuBufferData* data) {
    data->column_num_ = num;
  }

  const GpuBufferOptions& options() const { return options_;}
 protected:
  const GpuBufferOptions options_;
  DISALLOW_COPY_AND_ASSIGN(GpuBuffer);
};

class AZER_EXPORT AutoBufferLock {
 public:
  AutoBufferLock(GpuBuffer* hbuffer, MapType flags, Renderer* renderer)
      : hbuffer_(hbuffer) {
    data_ = hbuffer_->map(flags);
  }
  ~AutoBufferLock() {
    if (data_.get()) {
      hbuffer_->unmap();
    }
  }
 private:
  GpuBufferDataPtr data_;
  GpuBuffer* hbuffer_;
  DISALLOW_COPY_AND_ASSIGN(AutoBufferLock);
};

AZER_EXPORT const GpuBufferOptions& kVertexBufferOpt();
AZER_EXPORT const GpuBufferOptions& kIndicesBufferOpt();
AZER_EXPORT const GpuBufferOptions& kShaderConstsTableBufferOpt();
}  // namespace azer
