#include "azer/files/res_path_tokenizer.h"


namespace azer {
CharType ResPathTokenizer::kValidCharInPath[] = FILE_PATH_LITERAL("_.");

ResPathTokenizer::ResPathTokenizer(const StringType& string) 
    : raw_(string) {
  index_ = raw_.c_str();
}

ResPathTokenizer::~ResPathTokenizer() {
}

int ResPathTokenizer::GetNext() {
  CharType prev = FILE_PATH_LITERAL('\0');
  CharType c = *index_;
  if (c == FILE_PATH_LITERAL('\0')) {
    return kNoTokens;
  }

  current_.clear();
  while (c) {
    switch (c) {
      case FILE_PATH_LITERAL(':'): 
      case FILE_PATH_LITERAL('/'): 
      case FILE_PATH_LITERAL('.'): 
        if (!current_.empty() && prev != c) {
          return kSuccess;
        } else {
          current_.push_back(c);
        }
        break;
      default:
        current_.push_back(c);
        break;
    }
    prev = c;
    c = *++index_;
  }

  return kSuccess;
}

bool ResPathTokenizer::ValidStringBeginChar(CharType cb) const {
  if (cb >= FILE_PATH_LITERAL('a') && cb <= FILE_PATH_LITERAL('z')) {
    return true;
  }

  if (cb >= FILE_PATH_LITERAL('A') && cb <= FILE_PATH_LITERAL('Z')) {
    return true;
  }

  for (size_t i = 0; i < arraysize(kValidCharInPath); ++i) {
    if (kValidCharInPath[i] == cb) { return true;}
  }

  return false;
}


bool ResPathTokenizer::ValidStringFollowingChar(CharType cb) const {
  if (cb >= FILE_PATH_LITERAL('0') && cb <= FILE_PATH_LITERAL('9')) {
    return true;
  }
  return ValidStringBeginChar(cb);
}
}  // namespace azer
