#include "azer/render/gpu_buffer.h"

namespace azer {
GpuBufferData::GpuBufferData()
    : data_(0), row_size_(-1), size_(-1) {
}

GpuBufferOptions::GpuBufferOptions()
    : usage(kBufferDefault),
      cpu_access(kCPUNoAccess),
      target(kBindTargetUnknown) {
  memset(name, 0, sizeof(name));
}

GpuBuffer::GpuBuffer(const GpuBufferOptions& opt) 
    : options_(opt) {
}

GpuBuffer::~GpuBuffer() {
}

const GpuBufferOptions& kVertexBufferOpt() {
  static GpuBufferOptions opt;
  opt.target = kBindTargetVertexBuffer;
  return opt;
}

const GpuBufferOptions& kIndicesBufferOpt() {
  static GpuBufferOptions opt;
  opt.target = kBindTargetIndicesBuffer;
  return opt;
}

const GpuBufferOptions& kShaderConstsTableBufferOpt() {
  static GpuBufferOptions opt;
  opt.target = kBindTargetContantBuffer;
  return opt;
}
}  // namespace azer
