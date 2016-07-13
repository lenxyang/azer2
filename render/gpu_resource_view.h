#pragma once

#include "azer/render/resource_view.h"

namespace azer {
class GpuBuffer;
typedef scoped_refptr<GpuBuffer> GpuBufferPtr;

class AZER_EXPORT ShaderResView : public ResourceView {
 public:
  ShaderResView(ResViewType type, GpuBuffer* buffer);
  GpuBuffer* buffer() { return buffer_.get();}
 private:
  GpuBufferPtr buffer_;
  DISALLOW_COPY_AND_ASSIGN(ShaderResView);
};

class AZER_EXPORT UnorderAccessResView : public ResourceView {
 public:
  explicit UnorderAccessResView(GpuBuffer* buffer);
  UnorderAccessResView(ResViewType type, GpuBuffer* buffer);
  GpuBuffer* buffer() { return buffer_.get();}
 private:
  GpuBufferPtr buffer_;
  DISALLOW_COPY_AND_ASSIGN(UnorderAccessResView);
};

typedef scoped_refptr<ShaderResView> ShaderResViewPtr;
typedef scoped_refptr<UnorderAccessResView> UnorderAccessResViewPtr;
}  // namespace azer
