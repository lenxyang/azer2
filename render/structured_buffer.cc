#include "azer/render/structured_buffer.h"

#include "azer/render/gpu_buffer.h"
#include "azer/render/render_system.h"

namespace azer {
// class StructuredGpuBuffer
StructuredGpuBuffer::StructuredGpuBuffer(int count, int strip)
    : size_(count * strip),
      strip_(strip),
      count_(count) {
  static GpuResOptions resopt;
  resopt.target = kBindTargetShaderResource | kBindTargetUnorderAccess;
  resopt.type = GpuResType::kStructuredBuffer;
  InitGpuBuffer(resopt, count, strip);
}
StructuredGpuBuffer::StructuredGpuBuffer(const GpuResOptions& options, 
                                         int count, int strip)
    : size_(count * strip),
      strip_(strip),
      count_(count) {
}

StructuredGpuBuffer::~StructuredGpuBuffer() {
}

void StructuredGpuBuffer::InitGpuBuffer(const GpuResOptions& options,
                                        int count, int strip) {
  RenderSystem* rs = RenderSystem::Current();
  CHECK(rs) << "RenderSystem Not Initialized";
  gpu_buffer_ = rs->CreateBuffer(options, 1, size());
  CHECK(gpu_buffer_.get()) << "Initializer GpuBuffer for ShaderParam Failed";
}
}  // namespace azer
