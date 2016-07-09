#pragma once

#include "azer/render/gpu_buffer.h"
#include "azer/render/resource_view.h"

namespace azer {

class StructuredGpuBuffer;
typedef scoped_refptr<StructuredGpuBuffer> StructuredGpuBufferPtr;

class StructuredGpuBuffer : public GpuBuffer {
 public:
  ~StructuredGpuBuffer() override;

  int size() const { return size_;}
  int count() const { return count_;}
  int strip() const { return strip_;}

  virtual GpuBufferDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;
 private:
  StructuredGpuBuffer(const GpuBufferOptions& options, int count, int strip);

  const int size_;
  const int strip_;
  const int count_;
  DISALLOW_COPY_AND_ASSIGN(StructuredGpuBuffer);
};

class StructuredGpuBufferView : public ResourceView {
 public:
  struct Options {
    int type;
  };
  ~StructuredGpuBufferView() override;

  const Options& options() const { return options_;}
  StructuredGpuBuffer* buffer();
 protected:
  StructuredGpuBufferView(const Options& opt, StructuredGpuBuffer* buffer);
 private:
  const Options options_;
  scoped_refptr<StructuredGpuBuffer> buffer_;
  DISALLOW_COPY_AND_ASSIGN(StructuredGpuBufferView);
};
}  // namepace azer
