#pragma once

#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/native_types.h"

namespace azer {
enum class ResViewType {
  kRenderTarget = 1,
  kDepthBuffer,
  kTexture,
  kUATexture,
  kBufferResView,
  kBufferUAView,
};

class AZER_EXPORT ResourceView : public ::base::RefCounted<ResourceView> {
 public:
  explicit ResourceView(ResViewType type) : type_(type){}
  virtual ~ResourceView() {}

  ResViewType view_type() const { return type_;}
  virtual NativeGpuResourceViewHandle native_handle() = 0;
 private:
  ResViewType type_;
  DISALLOW_COPY_AND_ASSIGN(ResourceView);
};

typedef scoped_refptr<ResourceView> ResourceViewPtr;
}  // namespace azer
