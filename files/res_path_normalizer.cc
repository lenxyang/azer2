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
  typedef ::base::StringTokenizerT<StringType, StringType::const_iterator>
      StringTokenizer;
  StringTokenizer t(raw_, AZER_LITERAL("/:"));
  t.set_options(StringTokenizer::RETURN_DELIMS);
  while (t.GetNext()) {
    const StringType& token = t.token();
    State state = state();
    if (token == AZER_LITERAL("/")) {
      switch (state) {
        case kStart:
          set_state(kRootSlashBegin);
        continue;
        case kRootSlashBegin:
          set_state(kRootSlashEnd);
        proto_ = AZER_LITERAL("//");
        continue;
        case kString:
        set_state(kSlash1);
        continue;
        case kSlash1:
        set_state(kSlash2);
        continue;
        case kComma:
          set_state(kProtoSlahBegin);
          continue;
        case kProtoSlahBegin:
          set_state(kProtoSlahEnd);
          if (dirs_.size() > 1u || !proto_.empty()) {
            SetErrorMsg("invalid proto path");
            return;
          } else {
            proto_ = dirs_.back();
            proto_.append(AZER_LITERAL("://"));
            dirs_.clear();
          }
          continue;
        default:
          continue;
      }
    } else if (token == AZER_LITERAL(":")) {
      set_state(kComma);
      continue;
    } else if (token == AZER_LITERAL(".")) {
      switch (state) {
        case kComma:
          StringType str = AZER_LITERAL(".");
          set_state(kNameDot);
          component_.append(AZER_LITERAL("."));
          continue;
        case kNameDot:
          component_.append(AZER_LITERAL("."));
          continue;
        case kProtoSlahEnd:
        case kSlash1:
        case kSlash2:
          continue;
        case kStart:
          set_state(kDot1);
          continue;
        case kDot1:
          set_state(kDot2);
          continue;
        case kDot2:
          set_state(kDot2);
          continue;
        default:
          CHECK(false);
      }
    } else {
      switch (state) {
        case kStart:
        case kProtoSlahEnd:
        case kSlash1:
        case kSlash2:
        case kProtoSlahEnd:
          dirs_.push_back(token);
          set_state(kString);
          continue;
        case kDot1:
        case kDot2:
          dirs_.back().append(token);
          set_state(kString);
          continue;
        case kNameDot:
          component_.append(token);
          continue;
        case kComma:
          set_state(kString);
          if (component_.empty()) {
            state_ = kSlash1;
            component_ = token;
            SetErrorMsg("multiple component specified.");
            continue;
          } else {
            return;
          }
        default:
          CHECK(false);
      }
    }
  }
}

void ResPathNormalizer::SetErrorMsg(const std::string& msg) {
  error_msg_ = msg;
  set_state(kFailed);
}
}  // namespace azer
