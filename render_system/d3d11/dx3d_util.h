#pragma once

#include <algorithm>
#include <memory>
#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "azer/base/string.h"

#include <d3d11.h>

namespace azer {
namespace d3d11 {
std::string HRMessage(uint32_t hr);

struct COMDeleter {
  template<typename T> void operator()(T* ptr) {
    ptr->Release();
  }
};

typedef std::unique_ptr<ID3DBlob, COMDeleter> D3DBlobPtr;
typedef std::unique_ptr<IUnknown, COMDeleter> D3DObjPtr;

inline uint32_t Align16BindWidth(uint32_t size) {
  if (size & 0x000F) {
    size &= 0xFFF0;
    size += 0x0010;
  }
  return std::min(size, (uint32)D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT);
}

class ScopedRefCOM {
 public:
  ScopedRefCOM(IUnknown* ptr)
      : ptr_(ptr){
    DCHECK(ptr_ != NULL);
  }

  ~ScopedRefCOM() {
    DCHECK(ptr_ != NULL);
    ptr_->Release();
  }
 private:
  IUnknown* ptr_;
  DISALLOW_COPY_AND_ASSIGN(ScopedRefCOM);
};

class D3DRenderSystem;
void ReportDeviceObjects(D3DRenderSystem* rs);
}  // namespace d3d11
}  // namespace base

#define SAFE_RELEASE(res)                       \
  if ((res)) {                                  \
    (res)->Release();                           \
    (res) = NULL;                               \
  }

#define HRESULT_HANDLE(HR, LEVEL, MSG)                          \
  if (FAILED(HR)) {                                             \
    LOG(##LEVEL) << ##MSG << "(" << HRMessage(HR) << ")";       \
        return false;                                           \
  }

#define HRESULT_HANDLE_NORET(HR, LEVEL, MSG)                    \
  if (FAILED(HR)) {                                             \
    LOG(##LEVEL) << ##MSG << "(" << HRMessage(HR) << ")";       \
  }
