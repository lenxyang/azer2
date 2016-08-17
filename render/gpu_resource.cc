#include "azer/render/gpu_resource.h"

namespace azer {
GpuResLockData::GpuResLockData(uint8_t* data, int row_size, int column,
                               ::base::Callback<void()> callback)
    : data_(data), row_size_(row_size), column_num_(column),
      release_callback_(callback) {
}

GpuResLockData::~GpuResLockData() {
  DCHECK(!release_callback_.is_null());
  release_callback_.Run();
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
