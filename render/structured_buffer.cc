#include "azer/render/structured_buffer.h"

namespace azer {
StructuredGpuBuffer::StructuredGpuBuffer(const GpuBufferOptions& options, 
                                         int count, int strip)
    : GpuBuffer(options, GpuBufferType::kStructuredBuffer),
      size_(count * strip),
      strip_(strip),
      count_(count) {
}

StructuredGpuBuffer::~StructuredGpuBuffer() {
}

}  // namespace azer
