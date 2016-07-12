#pragma once

#include "azer/render/resource_view.h"

namespace azer {
class GpuBuffer;
typedef scoped_refptr<GpuBuffer> GpuBufferPtr;

class AZER_EXPORT ShaderResView : public ResourceView {
 public:
  ShaderResView(GpuBuffer* buffer);
 private:
  GpuBufferPtr buffer_;
  DISALLOW_COPY_AND_ASSIGN(ShaderResView);
};

class AZER_EXPORT UnorderAccessResView : public ResourceView {
 public:
  UnorderAccessResView(GpuBuffer* buffer);
 private:
  GpuBufferPtr buffer_;
  DISALLOW_COPY_AND_ASSIGN(UnorderAccessResView);
};

typedef scoped_refptr<ShaderResView> ShaderResViewPtr;
typedef scoped_refptr<UnorderAccessResView> UnorderAccessResViewPtr;
}  // namespace azer
