#pragma once

#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {
class AZER_EXPORT ResourceView : public ::base::RefCounted<ResourceView> {
 public:
  enum Type {
    kRenderTargetView,
    kDepthBufferView,
    kTextureView,
    kStructuredBufferView,
  };
  
  explicit ResourceView(Type type) : type_(type){}
  virtual ~ResourceView() {}

  Type view_type() const { return type_;}
  virtual NativeGpuResourceViewHandler native_handle() = 0;
 private:
  Type type_;
  DISALLOW_COPY_AND_ASSIGN(ResourceView);
};

typedef scoped_refptr<ResourceView> ResourceViewPtr;
}  // namespace azer
