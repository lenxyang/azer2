#pragma once

#include "azer/render/gpu_buffer.h"
#include "azer/render/resource_view.h"

namespace azer {
class StructuredGpuBufferView;
class StructuredGpuBuffer;
typedef scoped_refptr<StructuredGpuBuffer> StructuredGpuBufferPtr;
typedef scoped_refptr<StructuredGpuBufferView> StructuredGpuBufferViewPtr;

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
