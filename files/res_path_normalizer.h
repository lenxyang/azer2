#pragma once

#include <string>

#include "base/basictypes.h"
#include "azer/base/string.h"
#include "azer/files/res_path.h"

namespace azer {
class AZER_EXPORT ResPathNormalizer {
 public:
  explicit ResPathNormalizer(const StringType& raw_path);
  bool Normalize(ResPath* path);
 private:
  StringType proto_;
  StringType component_;
  StringType raw_path_;
  DISALLOW_COPY_AND_ASSIGN(ResPathNormalizer);
};
}  // namespace azer
