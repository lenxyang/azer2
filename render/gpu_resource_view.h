#pragma once

#include "azer/render/resource_view.h"

namespace azer {
class GpuResource;
typedef scoped_refptr<GpuResource> GpuResourcePtr;

class AZER_EXPORT ShaderResView : public ResourceView {
 public:
  ShaderResView(ResViewType type, GpuResource* buffer);
  GpuResource* buffer() { return buffer_.get();}
 private:
  GpuResourcePtr buffer_;
  DISALLOW_COPY_AND_ASSIGN(ShaderResView);
};

class AZER_EXPORT UnorderAccessView : public ResourceView {
 public:
  explicit UnorderAccessView(GpuResource* buffer);
  UnorderAccessView(ResViewType type, GpuResource* buffer);
  GpuResource* buffer() { return buffer_.get();}
 private:
  GpuResourcePtr buffer_;
  DISALLOW_COPY_AND_ASSIGN(UnorderAccessView);
};

typedef scoped_refptr<ShaderResView> ShaderResViewPtr;
typedef scoped_refptr<UnorderAccessView> UnorderAccessViewPtr;
}  // namespace azer
