#pragma once

#include "azer/render/gpu_resource.h"

namespace azer {
class StructuredGpuBuffer;
typedef scoped_refptr<StructuredGpuBuffer> StructuredGpuBufferPtr;

class AZER_EXPORT StructuredGpuBuffer : public GpuBuffer {
 public:
  ~StructuredGpuBuffer() override;

  int size() const { return size_;}
  int count() const { return count_;}
  int strip() const { return strip_;}

  virtual GpuResLockDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;
 protected:
  StructuredGpuBuffer(const GpuResOptions& options, int count, int strip);
 private:
  const int size_;
  const int strip_;
  const int count_;
  DISALLOW_COPY_AND_ASSIGN(StructuredGpuBuffer);
};

}  // namepace azer
