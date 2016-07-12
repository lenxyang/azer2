#include "azer/render/gpu_buffer_view.h"

#include "azer/render/gpu_buffer.h"

namespace azer {
ShaderResView::ShaderResView(ResViewType type, GpuBuffer* buffer)
    : ResourceView(type),
      buffer_(buffer) {
  DCHECK(buffer->buffer_options().target & kBindTargetShaderResource);
}


UnorderAccessResView::UnorderAccessResView(ResViewType type, GpuBuffer* buffer)
    : ResourceView(type),
      buffer_(buffer) {
  DCHECK(buffer->buffer_options().target & kBindTargetUnorderedAccess);
}
}  // namespace azer
