#pragma once

#include "azer/render/gpu_buffer.h"

namespace azer {
class StructuredGpuBuffer;
typedef scoped_refptr<StructuredGpuBuffer> StructuredGpuBufferPtr;

// class StructuredGpuBuffer
class AZER_EXPORT StructuredGpuBuffer :
      public ::base::RefCounted<StructuredGpuBuffer> {
 public:
  StructuredGpuBuffer(int count, int strip);
  StructuredGpuBuffer(const GpuResOptions& options, int count, int strip);
  ~StructuredGpuBuffer();

  int size() const { return size_;}
  int count() const { return count_;}
  int strip() const { return strip_;}
  GpuBuffer* gpu_buffer() { return gpu_buffer_.get();}
 private:
  void InitGpuBuffer(const GpuResOptions& options, int count, int strip);

  const int size_;
  const int strip_;
  const int count_;
  GpuBufferPtr gpu_buffer_;
  DISALLOW_COPY_AND_ASSIGN(StructuredGpuBuffer);
};

}  // namepace azer
