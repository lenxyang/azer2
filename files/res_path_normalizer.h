#pragma once

#include "base/basictypes.h"
#include "azer/base/string.h"
#include "azer/files/res_path.h"

namespace azer {
class AZER_EXPORT ResPathNormalizer {
 public:
  explicit ResPathNormalizer(const StringType& path);

  void Normalize();

  const StringType& normalized() const;
  bool success() const;
 private:
  enum State {
    kStart,
    kFailed,
    kFinished,
  };

  std::vector<StringType> tokens_;
  StringType raw_;
  StringType normalized_;
  State state_;

  static CharType kValidCharInPath[];
  DISALLOW_COPY_AND_ASSIGN(ResPathNormalizer);
};
}  // namespace azer
