#include "azer/render/gpu_resource.h"

namespace azer {
GpuResLockData::GpuResLockData(uint8_t* data, int row_size, int column)
    : data_(data), row_size_(row_size), column_num_(column) {
}

// class GpuResOptions
GpuResOptions::GpuResOptions()
    : usage(kBufferDefault),
      cpu_access(kCPUNoAccess),
      target(kBindTargetUnknown) {
  memset(name, 0, sizeof(name));
}

GpuBuffer::GpuBuffer(const GpuResOptions& opt, GpuResType type) 
    : buffer_options_(opt),
      buffer_type_(type) {
}

GpuBuffer::~GpuBuffer() {
}

const GpuResOptions& kVertexBufferOpt() {
  static GpuResOptions opt;
  opt.target = kBindTargetVertexBuffer;
  return opt;
}

const GpuResOptions& kIndicesBufferOpt() {
  static GpuResOptions opt;
  opt.target = kBindTargetIndicesBuffer;
  return opt;
}

const GpuResOptions& kShaderConstsTableBufferOpt() {
  static GpuResOptions opt;
  opt.target = kBindTargetContantBuffer;
  return opt;
}
}  // namespace azer
