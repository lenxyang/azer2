#pragma once

#include <memory>


#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/common.h"

namespace azer {

class Renderer;
class GpuResLockData;
class GpuBuffer;
typedef scoped_refptr<GpuResLockData> GpuResLockDataPtr;
typedef scoped_refptr<GpuBuffer> GpuBufferPtr;

class AZER_EXPORT GpuResLockData: public ::base::RefCounted<GpuResLockData> {
 public:
  GpuResLockData(uint8_t* data, int row_size, int column);
  uint8_t* data_ptr() const { return data_;}
  int row_size() const { return row_size_;}
  int column_num() const { return column_num_;}
  int size() const { return row_size_ * column_num_;}
 private:
  uint8_t* data_;
  int row_size_;
  int column_num_;
  int size_;
  DISALLOW_COPY_AND_ASSIGN(GpuResLockData);
};

struct AZER_EXPORT GpuResOptions {
  char name[128];
  BufferUsage usage;
  CPUAccess cpu_access;  // defined render_system
  uint32_t target;
  GpuResOptions();
};

enum class GpuResType {
  kVertexBuffer = 1,
  kIndicesBuffer,
  kStructuredBuffer,
  kConstantTable,
  kTexture,
};

class AZER_EXPORT GpuBuffer : public ::base::RefCounted<GpuBuffer> {
 public:
  GpuBuffer(const GpuResOptions& opt, GpuResType type);
  virtual ~GpuBuffer();

  GpuResType buffer_type() const { return buffer_type_;}

  virtual GpuResLockDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;
  virtual NativeGpuResourceHandle native_handle() = 0;
  const GpuResOptions& buffer_options() const { return buffer_options_;}
 private:
  const GpuResOptions buffer_options_;
  const GpuResType buffer_type_;
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
  GpuResLockDataPtr data_;
  GpuBuffer* hbuffer_;
  DISALLOW_COPY_AND_ASSIGN(AutoBufferLock);
};

AZER_EXPORT const GpuResOptions& kVertexBufferOpt();
AZER_EXPORT const GpuResOptions& kIndicesBufferOpt();
AZER_EXPORT const GpuResOptions& kShaderConstsTableBufferOpt();
}  // namespace azer
