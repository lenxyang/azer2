#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "azer/base/string.h"

namespace azer {
std::string HRMessage(uint32 hr);
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
