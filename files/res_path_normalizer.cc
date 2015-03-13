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
  /*
  ResPathTokenizer tokenizer(raw_);
  ResPathTokenizer::TokenType prev_token_type = ResPathTokenizer::kUnknownToken;
  int ret = ResPathTokenizer::kSuccess;
  StringType token
  StringType prev_token;
  while ((ret = tokenizer.GetNext()) != kSuccess) {
    prev_token_type = token_type;
    ResPathTokenizer::TokenType token_type = tokenizer.token_type();
    prev_token = token;
    token = tokenizer.token();
    switch (token_type) {
      case ResPathTokenizer::kCommaToken:
        if (prev == kStringToken) 
          set_state(kComponentState);
        else
          return false;
        break;
      case ResPathTokenizer::kSlashToken:
        break;
      case ResPathTokenizer::kDotToken:
        break;
      case ResPathTokenizer::kStringToken:
        current_.append(token);
        if (state() == kComponentState) {
        }
        break;
    }
  }

  return ret == ResPathNormalizer::kNoTokens;
  */
  return true;
}

void ResPathNormalizer::SetErrorMsg(const std::string& msg) {
  error_msg_ = msg;
  set_state(kFailed);
}

void ResPathNormalizer::Apply(ResPath* respath) {
}
}  // namespace azer
