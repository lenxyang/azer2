#include "azer/files/res_path_normalizer.h"

#include "base/logging.h"
#include "base/strings/string_split.h"
#include "base/strings/string_tokenizer.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"

namespace azer {
CharType ResPathNormalizer::kValidCharInPath[] = FILE_PATH_LITERAL("_:/.");

ResPathNormalizer::ResPathNormalizer(const StringType& path)
    : raw_(path)
    , state_(kStart) {
}

const StringType& ResPathNormalizer::normalized() const {
  DCHECK(success());
  return normalized_;
}

bool ResPathNormalizer::success() const {
  return (state_ == kFinished);
}

void ResPathNormalizer::Normalize() {
}
}  // namespace azer
