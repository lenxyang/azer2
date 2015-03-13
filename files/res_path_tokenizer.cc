#include "azer/files/res_path_tokenizer.h"


namespace azer {
ResPathSplitter::ResPathSplitter(const StringType& string) 
    : raw_(string) {
  index_ = raw_.c_str();
}

ResPathSplitter::~ResPathSplitter() {
}

int ResPathSplitter::GetNext() {
  CharType prev = FILE_PATH_LITERAL('\0');
  CharType c = *index_;
  if (c == FILE_PATH_LITERAL('\0')) {
    token_type_ = kUnkownToken;
    return kNoTokens;
  }

  bool demils = false;
  current_.clear();
  while (c) {
    switch (c) {
      case FILE_PATH_LITERAL(':'): 
      case FILE_PATH_LITERAL('/'): 
      case FILE_PATH_LITERAL('.'): 
        demils = true;
        token_type_ = static_cast<Type>(c);
        if (!current_.empty() && prev != c) {
          return kSuccess;
        } else {
          current_.push_back(c);
        }
        break;
      default:
        if (demils) return kSuccess;
        token_type_ = kStringToken;
        current_.push_back(c);
        if (!ValidStringChar(c)) {
          ++index_;
          return kContainInvalidChar;
        }
        break;
    }
    prev = c;
    c = *++index_;
  }

  return kSuccess;
}

bool ResPathSplitter::ValidStringChar(CharType cb) const {
  if (cb >= FILE_PATH_LITERAL('a') && cb <= FILE_PATH_LITERAL('z')) {
    return true;
  }

  if (cb >= FILE_PATH_LITERAL('A') && cb <= FILE_PATH_LITERAL('Z')) {
    return true;
  }

  if (cb >= FILE_PATH_LITERAL('0') && cb <= FILE_PATH_LITERAL('9')) {
    return true;
  }

  if (cb == '_') { return true;}
  return false;
}

}  // namespace azer
