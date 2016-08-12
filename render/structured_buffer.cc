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
  InitGpuBuffer(resopt, count, strip, NULL);
}

StructuredGpuBuffer::StructuredGpuBuffer(int count, int strip, const uint8_t* data)
    : size_(count * strip),
      strip_(strip),
      count_(count) {
  static GpuResOptions resopt;
  resopt.target = kBindTargetShaderResource | kBindTargetUnorderAccess;
  resopt.type = GpuResType::kStructuredBuffer;
  InitGpuBuffer(resopt, count, strip, data);
}


StructuredGpuBuffer::StructuredGpuBuffer(const GpuResOptions& resopt, 
                                         int count, int strip)
    : size_(count * strip),
      strip_(strip),
      count_(count) {
  DCHECK(resopt.type == GpuResType::kStructuredBuffer);
  InitGpuBuffer(resopt, count, strip, NULL);
}

StructuredGpuBuffer::StructuredGpuBuffer(const GpuResOptions& resopt, 
                                         int count, int strip, const uint8_t* data)
    : size_(count * strip),
      strip_(strip),
      count_(count) {
  DCHECK(resopt.type == GpuResType::kStructuredBuffer);
  InitGpuBuffer(resopt, count, strip, data);
}

StructuredGpuBuffer::~StructuredGpuBuffer() {
}

void StructuredGpuBuffer::InitGpuBuffer(const GpuResOptions& options,
                                        int count, int strip,
                                        const uint8_t* data) {
  RenderSystem* rs = RenderSystem::Current();
  CHECK(rs) << "RenderSystem Not Initialized";
  gpu_buffer_ = rs->CreateBuffer(options, count, strip, data);
  CHECK(gpu_buffer_.get()) << "Initializer GpuBuffer for ShaderParam Failed";
}
}  // namespace azer
