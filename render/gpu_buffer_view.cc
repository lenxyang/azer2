#include "azer/render/gpu_buffer_view.h"

#include "azer/render/gpu_buffer.h"

namespace azer {
ShaderResView::ShaderResView(const Options& options, GpuBuffer* buffer)
    : buffer_(buffer),
      options_(options) {
  DCHECK(buffer->buffer_options().target & kBindTargetShaderResource);
}

UnorderAccessResView::UnorderAccessResView(const Options& options, GpuBuffer* buffer)
    : buffer_(buffer),
      options_(options) {
  DCHECK(buffer->buffer_options().target & kBindTargetUnorderedAccess);
}
}  // namespace azer
