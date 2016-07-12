#pragma once

#include "azer/render/resource_view.h"

namespace azer {
class GpuBuffer;
typedef scoped_refptr<GpuBuffer> GpuBufferPtr;

class AZER_EXPORT ShaderResView : public ResourceView {
 public:
  struct AZER_EXPORT Options {
    int type;
    Options();
  };

  ShaderResView(const Options& options, GpuBuffer* buffer);
  const Options& options() const { return options_;}
 private:
  GpuBufferPtr buffer_;
  const Options options_;
  DISALLOW_COPY_AND_ASSIGN(ShaderResView);
};

class AZER_EXPORT UnorderAccessResView : public ResourceView {
 public:
  struct AZER_EXPORT Options {
    int type;
    Options();
  };

  UnorderAccessResView(const Options& options, GpuBuffer* buffer);
  const Options& options() const { return options_;}
 private:
  GpuBufferPtr buffer_;
  const Options options_;
  DISALLOW_COPY_AND_ASSIGN(UnorderAccessResView);
};
}  // namespace azer
