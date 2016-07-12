#include "azer/render/gpu_buffer.h"

namespace azer {
GpuBufferLockData::GpuBufferLockData(uint8_t* data, int row_size, int column)
    : data_(data), row_size_(row_size), column_num_(column) {
}

// class GpuBufferOptions
GpuBufferOptions::GpuBufferOptions()
    : usage(kBufferDefault),
      cpu_access(kCPUNoAccess),
      target(kBindTargetUnknown) {
  memset(name, 0, sizeof(name));
}

GpuBuffer::GpuBuffer(const GpuBufferOptions& opt) 
    : buffer_options_(opt) {
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
