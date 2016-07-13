#include "azer/render/structured_buffer.h"

namespace azer {
StructuredGpuBuffer::StructuredGpuBuffer(const GpuResOptions& options, 
                                         int count, int strip)
    : GpuResource(options, GpuResType::kStructuredBuffer),
      size_(count * strip),
      strip_(strip),
      count_(count) {
}

StructuredGpuBuffer::~StructuredGpuBuffer() {
}

}  // namespace azer
