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
}

GpuResource::GpuResource(const GpuResOptions& opt, GpuResType type) 
    : resource_options_(opt),
      resource_type_(type) {
}

GpuResource::~GpuResource() {
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

std::ostream& operator << (std::ostream& os, const GpuResType& res) {
  switch (res) {
    case GpuResType::kVertexBuffer: os << "VertexBufferRes"; break;
    case GpuResType::kIndicesBuffer: os << "IndexBufferRes"; break;
    case GpuResType::kStructuredBuffer: os << "StructureBufferRes"; break;
    case GpuResType::kConstantTable: os << "ConstantTableRes"; break;
    case GpuResType::kTexture: os << "TextureRes"; break;
    default: CHECK(false) << "No such GpuResType: " << (int)res;
  }
  return os;
}
}  // namespace azer
