#include "azer/render/gpu_buffer.h"

namespace azer {
// class GpuBuffer
GpuBuffer::GpuBuffer(const GpuResOptions& options, int count, int strip)
    : GpuResource(options, GpuResType::kUnknown),
      size_(count * strip),
      strip_(strip),
      count_(count) {
}

GpuBuffer::~GpuBuffer() {
}

}  // namespace azer
