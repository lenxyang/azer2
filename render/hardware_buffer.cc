#include "azer/render/hardware_buffer.h"

namespace azer {
HardwareBufferData::HardwareBufferData()
    : data_(0), row_size_(-1), size_(-1) {
}

HBufferOptions::HBufferOptions()
    : usage(kBufferDefault),
      cpu_access(kCPUNoAccess),
      target(kBindTargetUnknown) {
  memset(name, 0, sizeof(name));
}

HardwareBuffer::HardwareBuffer(const HBufferOptions& opt) 
    : options_(opt) {
}

HardwareBuffer::~HardwareBuffer() {
}

const HBufferOptions& kVertexBufferOpt() {
  static HBufferOptions opt;
  opt.target = kBindTargetVertexBuffer;
  return opt;
}

const HBufferOptions& kIndicesBufferOpt() {
  static HBufferOptions opt;
  opt.target = kBindTargetIndicesBuffer;
  return opt;
}

const HBufferOptions& kShaderConstsTableBufferOpt() {
  static HBufferOptions opt;
  opt.target = kBindTargetContantBuffer;
  return opt;
}
}  // namespace azer
