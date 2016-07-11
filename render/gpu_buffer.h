#pragma once

#include <memory>


#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/common.h"

namespace azer {

class Renderer;
class GpuBufferLockData;
class GpuBuffer;
typedef scoped_refptr<GpuBufferLockData> GpuBufferLockDataPtr;

class AZER_EXPORT GpuBufferLockData: public ::base::RefCounted<GpuBufferLockData> {
 public:
  GpuBufferLockData(uint8_t* data, int row_size, int column);
  uint8_t* data_ptr() const { return data_;}
  int row_size() const { return row_size_;}
  int column_num() const { return column_num_;}
  int size() const { return row_size_ * column_num_;}
 private:
  uint8_t* data_;
  int row_size_;
  int column_num_;
  int size_;
  DISALLOW_COPY_AND_ASSIGN(GpuBufferLockData);
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

  virtual GpuBufferLockDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;
 protected:
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
  GpuBufferLockDataPtr data_;
  GpuBuffer* hbuffer_;
  DISALLOW_COPY_AND_ASSIGN(AutoBufferLock);
};

AZER_EXPORT const GpuBufferOptions& kVertexBufferOpt();
AZER_EXPORT const GpuBufferOptions& kIndicesBufferOpt();
AZER_EXPORT const GpuBufferOptions& kShaderConstsTableBufferOpt();
}  // namespace azer
