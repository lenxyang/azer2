#include "azer/render/structured_buffer.h"

namespace azer {
StructuredGpuBuffer::StructuredGpuBuffer(const GpuBufferOptions& options, 
                                         int count, int strip)
    : GpuBuffer(options),
      size_(count * strip),
      strip_(strip),
      count_(count) {
}

StructuredGpuBuffer::~StructuredGpuBuffer() {
}

// class StructuredGpuBufferView
StructuredGpuBufferView::Options::Options() {
}

StructuredGpuBufferView::StructuredGpuBufferView(const Options& opt, 
                                           StructuredGpuBuffer* buffer)
    : ResourceView(ResourceView::kTextureView),
      options_(opt),
      buffer_(buffer) {
}

StructuredGpuBufferView::~StructuredGpuBufferView() {
}

StructuredGpuBuffer* StructuredGpuBufferView::buffer() { 
  return buffer_.get(); 
}
}  // namespace azer
