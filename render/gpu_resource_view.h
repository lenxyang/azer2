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

class AZER_EXPORT UnorderAccessResView : public ResourceView {
 public:
  explicit UnorderAccessResView(GpuResource* buffer);
  UnorderAccessResView(ResViewType type, GpuResource* buffer);
  GpuResource* buffer() { return buffer_.get();}
 private:
  GpuResourcePtr buffer_;
  DISALLOW_COPY_AND_ASSIGN(UnorderAccessResView);
};

typedef scoped_refptr<ShaderResView> ShaderResViewPtr;
typedef scoped_refptr<UnorderAccessResView> UnorderAccessResViewPtr;
}  // namespace azer
