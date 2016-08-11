#include "azer/render/gpu_resource.h"

namespace azer {
GpuResLockData::GpuResLockData(uint8_t* data, int row_size, int column)
    : data_(data), row_size_(row_size), column_num_(column) {
}

// class GpuResOptions
GpuResOptions::GpuResOptions()
    : usage(kBufferDefault),
      cpu_access(kCPUNoAccess),
      target(kBindTargetUnknown),
      type(GpuResType::kUnknown) {
}

GpuResource::GpuResource(const GpuResOptions& opt) 
    : resource_options_(opt) {
}

GpuResource::~GpuResource() {
}

std::ostream& operator << (std::ostream& os, const GpuResType& res) {
  switch (res) {
    case GpuResType::kVertexBuffer: os << "VertexBufferRes"; break;
    case GpuResType::kIndicesBuffer: os << "IndexBufferRes"; break;
    case GpuResType::kStructuredBuffer: os << "StructureBufferRes"; break;
    case GpuResType::kConstantTable: os << "kConstantTable"; break;
    case GpuResType::kTexture: os << "TextureRes"; break;
    default: CHECK(false) << "No such GpuResType: " << (int)res;
  }
  return os;
}
}  // namespace azer
