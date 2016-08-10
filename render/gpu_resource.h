#pragma once

#include <memory>


#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/common.h"

namespace azer {

class Renderer;
class GpuResLockData;
class GpuResource;
typedef scoped_refptr<GpuResLockData> GpuResLockDataPtr;
typedef scoped_refptr<GpuResource> GpuResourcePtr;

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
  BufferUsage usage;
  CPUAccess cpu_access;  // defined render_system
  uint32_t target;
  GpuResOptions();
};

enum class GpuResType {
  kVertexBuffer      = 0x0001,
  kIndicesBuffer     = 0x0002,
  kStructuredBuffer  = 0x0004,
  kConstantTable     = 0x0008,
  kTexture           = 0x0010,
};

AZER_EXPORT std::ostream& operator << (std::ostream& os, const GpuResType& res);

class AZER_EXPORT GpuResource : public ::base::RefCounted<GpuResource> {
 public:
  GpuResource(const GpuResOptions& opt, GpuResType type);
  virtual ~GpuResource();

  GpuResType resource_type() const { return resource_type_;}

  virtual void SetName(const std::string& name) = 0;
  virtual GpuResLockDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;
  virtual bool CopyTo(GpuResource* res) = 0;
  virtual NativeGpuResourceHandle native_handle() = 0;
  const GpuResOptions& resource_options() const { return resource_options_;}
 private:
  const GpuResOptions resource_options_;
  const GpuResType resource_type_;
  DISALLOW_COPY_AND_ASSIGN(GpuResource);
};

class AZER_EXPORT AutoBufferLock {
 public:
  AutoBufferLock(GpuResource* hbuffer, MapType flags, Renderer* renderer)
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
  GpuResource* hbuffer_;
  DISALLOW_COPY_AND_ASSIGN(AutoBufferLock);
};

AZER_EXPORT const GpuResOptions& kVertexBufferOpt();
AZER_EXPORT const GpuResOptions& kIndicesBufferOpt();
AZER_EXPORT const GpuResOptions& kShaderConstsTableBufferOpt();
}  // namespace azer
