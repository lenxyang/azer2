#include "azer/render/gpu_resource_view.h"

#include "base/logging.h"
#include "azer/render/gpu_resource.h"

namespace azer {
ShaderResView::ShaderResView(ResViewType type, GpuResource* buffer)
    : ResourceView(type),
      buffer_(buffer) {
  DCHECK(buffer->buffer_options().target & kBindTargetShaderResource);
}


UnorderAccessResView::UnorderAccessResView(ResViewType type, GpuResource* buffer)
    : ResourceView(type),
      buffer_(buffer) {
  DCHECK(buffer->buffer_options().target & kBindTargetUnorderedAccess);
}
}  // namespace azer
