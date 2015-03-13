#include "azer/files/res_path_normalizer.h"

#include "base/logging.h"
#include "base/strings/string_split.h"
#include "base/strings/string_tokenizer.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/base/string.h"
#include "azer/files/res_path_tokenizer.h"

namespace azer {
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

bool ResPathNormalizer::Normalize() {
  files::ResPathTokenizer tokenizer(raw_);
  while (tokenizer.GetNext() != kSuccess) {
    const StringType& token = tokenizer.token();
    CharType first = token[0];
    if (first == FILE_PATH_LITERAL(':')) {
    } else if (first == FILE_PATH_LITERAL('/')) {
    } else {
    }
  }

  return true;
}

void ResPathNormalizer::SetErrorMsg(const std::string& msg) {
  error_msg_ = msg;
  set_state(kFailed);
}

void ResPathNormalizer::Apply(ResPath* respath) {
}
}  // namespace azer
