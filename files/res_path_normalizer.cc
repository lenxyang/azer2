#include "azer/files/res_path_normalizer.h"

#include "base/logging.h"
#include "base/strings/string_split.h"
#include "base/strings/string_tokenizer.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/base/string.h"

namespace azer {
CharType ResPathNormalizer::kValidCharInPath[] = FILE_PATH_LITERAL("_.");
CharType ResPathNormalizer::kValidDelimsInPath[] = FILE_PATH_LITERAL(":/");

namespace {
bool ValidStringBeginChar(CharType cb) {
  if (cb >= FILE_PATH_LITERAL('a') && cb <= FILE_PATH_LITERAL('z')) {
    return true;
  }
  if (cb >= FILE_PATH_LITERAL('A') && cb <= FILE_PATH_LITERAL('Z')) {
    return true;
  }
  for (size_t i = 0; i < arraysize(ResPathNormalizer::kValidCharInPath); ++i) {
    if (ResPathNormalizer::kValidCharInPath[i] == cb) { return true;}
  }

  return false;
}

bool ValidStringFollowingChar(CharType cb) {
  if (cb >= FILE_PATH_LITERAL('0') && cb <= FILE_PATH_LITERAL('9')) {
    return true;
  }
  return ValidStringBeginChar(cb);
}

bool ValidString(const StringType& str) {
  auto iter = str.begin();
  if (!ValidStringBeginChar(*iter++)) { return false; }
  for (; iter != str.end(); ++iter) {
    if (!ValidStringFollowingChar(*iter)) { return false; }
  }
  return true;
}
}  // namespace

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

void ResPathNormalizer::SetErrorMsg(const std::string& msg) {
  error_msg_ = msg;
  set_state(kFailed);
}

void ResPathNormalizer::Apply(ResPath* respath) {
}
}  // namespace azer
