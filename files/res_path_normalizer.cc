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
    if (token == AZER_LITERAL("/")) {
      if (state() == kStart) {
        set_state(kRootSlashBegin);
        continue;
      } else if (state() == kRootSlashBegin) {
        set_state(kRootSlashEnd);
        proto_ = AZER_LITERAL("//");
        continue;
      } else if (state() == kString) {
        set_state(kSlash1);
        continue;
      } else if (state() == kSlash1) {
        set_state(kSlash2);
        continue;
      } else if (state() == kComma) {
        set_state(kProtoSlahBegin);
        continue;
      } else if (state() == kProtoSlahBegin) {
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
      }
    } else if (token == AZER_LITERAL(":")) {
      set_state(kComma);
      continue;
    } else if (token == AZER_LITERAL(".")) {
      if (state() == kComma) {
        StringType str = AZER_LITERAL(".");
        set_state(kNameDot);
        component_.append(AZER_LITERAL("."));
      } else if (state() == kNameDot) {
        component_.append(AZER_LITERAL("."));
      } else if (state() == kProtoSlahEnd
                 || state() == kSlash1
                 || state() == kSlash2) {
        
      } else if (state() == kStart) {
        set_state(kDot1);
      } else if (state() == kDot1) {
        set_state(kDot2);
      } else if (state() == kDot2) {
        set_state(kDot2);
      }
    } else {
      if (state() == kStart || state() == kProtoSlahEnd
          || state() == kSlash1 || state() == kSlash2) {
          
        dirs_.push_back(token);
        set_state(kString);
        continue;
      } else if (state() == kDot1 || state() == kDot2) {
        dirs_.back().append(token);
        set_state(kString);
        continue;
      } else if (state() == kNameDot) { 
        component_.append(token);
        continue;
      } else if (state() == kComma) {
        set_state(kString);
        if (component_.empty()) {
          state_ = kSlash1;
          component_ = token;
          SetErrorMsg("multiple component specified.");
          continue;
        } else {
          return;
        }
      } else {
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
