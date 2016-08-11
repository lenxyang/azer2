#include "azer/render/gpu_buffer.h"

#include "base/logging.h"

namespace azer {
// class GpuBuffer
GpuBuffer::GpuBuffer(const GpuResOptions& options, int count, int strip)
    : GpuResource(options),
      size_(count * strip),
      strip_(strip),
      count_(count) {
  CHECK(options.type != GpuResType::kUnknown);
}

GpuBuffer::~GpuBuffer() {
}

}  // namespace azer
