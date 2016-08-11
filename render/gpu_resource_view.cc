#include "azer/render/gpu_resource_view.h"

#include "base/logging.h"
#include "azer/render/gpu_resource.h"

namespace azer {
ShaderResView::ShaderResView(ResViewType type, GpuResource* buffer)
    : ResourceView(type),
      buffer_(buffer) {
  DCHECK(buffer->resource_options().target & kBindTargetShaderResource);
}


UnorderAccessView::UnorderAccessView(ResViewType type, GpuResource* buffer)
    : ResourceView(type),
      buffer_(buffer) {
  DCHECK(buffer->resource_options().target & kBindTargetUnorderAccess);
}
}  // namespace azer
