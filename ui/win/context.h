#pragma once

#include "azer/base/export.h"
#include "base/lazy_instance.h"

namespace azer {
namespace win {
class AZER_EXPORT WinContext {
 public:
  static WinContext* Pointer();
 private:
  DISALLOW_COPY_AND_ASSIGN(WinContext);
};
}  // namespace win
}  // namespace azer
