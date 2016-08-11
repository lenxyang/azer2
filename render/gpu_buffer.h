#pragma once

#include "azer/render/gpu_types.h"
#include "azer/render/gpu_resource.h"

namespace azer {
// class GpuBuffer
class AZER_EXPORT GpuBuffer : public GpuResource {
 public:
  ~GpuBuffer() override;

  int size() const { return size_;}
  int count() const { return count_;}
  int strip() const { return strip_;}

  virtual GpuResLockDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;
  virtual void UpdateData(const uint8_t* data, int64_t size) = 0;
 protected:
  GpuBuffer(const GpuResOptions& options, int count, int strip);
 private:
  const int size_;
  const int strip_;
  const int count_;
  DISALLOW_COPY_AND_ASSIGN(GpuBuffer);
};

}  // namespace azer
