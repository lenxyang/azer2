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

bool ResPathNormalizer::HandleToken(const StringType& token) {
  State cur_state = state();
  if (token == AZER_LITERAL("/")) {
    switch (cur_state) {
      case kStart:
        set_state(kRootSlashBegin);
        break;
      case kRootSlashBegin:
        set_state(kRootSlashEnd);
        proto_ = AZER_LITERAL("//");
        break;
      case kString:
        set_state(kSlash1);
        break;
      case kSlash1:
        set_state(kSlash2);
        break;
      case kComma:
        set_state(kProtoSlahBegin);
        break;
      case kProtoSlahBegin:
        set_state(kProtoSlahEnd);
        if (dirs_.size() > 1u || !proto_.empty()) {
          SetErrorMsg("invalid proto path");
          return false;
        } else {
          proto_ = dirs_.back();
          proto_.append(AZER_LITERAL("://"));
          dirs_.clear();
        }
        break;
      default:
        break;
    }
  } else if (token == AZER_LITERAL(":")) {
    set_state(kComma);
    return true;
  } else if (token == AZER_LITERAL(".")) {
    switch (cur_state) {
      case kComma:
        set_state(kNameDot);
        component_.append(AZER_LITERAL("."));
        break;
      case kNameDot:
        component_.append(AZER_LITERAL("."));
        break;
      case kProtoSlahEnd:
      case kSlash1:
      case kSlash2:
        break;
      case kStart:
        set_state(kDot1);
        break;
      case kDot1:
        set_state(kDot2);
        break;
      case kDot2:
        set_state(kDot2);
        break;
      default:
        CHECK(false);
        return false;
    }
  } else {
    switch (cur_state) {
      case kStart:
      case kProtoSlahEnd:
      case kRootSlashEnd:
      case kSlash1:
      case kSlash2:
        dirs_.push_back(token);
        set_state(kString);
        break;
      case kDot1:
      case kDot2:
        dirs_.back().append(token);
        set_state(kString);
        break;
      case kNameDot:
        component_.append(token);
        break;
      case kComma:
        set_state(kString);
        if (component_.empty()) {
          state_ = kSlash1;
          component_ = token;
          break;
        } else {
          SetErrorMsg("multiple component specified.");
          return false;
        }
      default:
        CHECK(false);
        return false;
    }
  }

  return true;
}

void ResPathNormalizer::Normalize() {
  typedef ::base::StringTokenizerT<StringType, StringType::const_iterator>
      StringTokenizer;
  StringTokenizer t(raw_, AZER_LITERAL("/:"));
  t.set_options(StringTokenizer::RETURN_DELIMS);
  while (t.GetNext()) {
    const StringType& token = t.token();
    if (!HandleToken(token)) {
      return;
    }
  }

  set_state(kFinished);
}

void ResPathNormalizer::SetErrorMsg(const std::string& msg) {
  error_msg_ = msg;
  set_state(kFailed);
}
}  // namespace azer
