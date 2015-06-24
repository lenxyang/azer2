#pragma once

#include <d3d11.h>

#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render_system/d3d11/util.h"

namespace azer {
namespace d3d11 {
class D3DRenderer;
class D3DRenderSystem;

class D3DGpuConstantsTable : public GpuConstantsTable {
 public:
  virtual ~D3DGpuConstantsTable() {
    SAFE_RELEASE(buffer_);
  }

  virtual void flush(Renderer*) override;
 private:
  D3DGpuConstantsTable(int32 num, const Desc* desc)
      : GpuConstantsTable(num, desc)
      , buffer_(NULL) {
  }
  
 private:
  ID3D11Buffer* buffer_;
  bool Init(D3DRenderSystem* rs);

  friend class D3DRenderSystem;
  friend class D3DRenderer;
  DISALLOW_COPY_AND_ASSIGN(D3DGpuConstantsTable);
};
}  // namespace d3d11
}  // namespace azer
