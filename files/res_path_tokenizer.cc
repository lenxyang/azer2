#include "azer/files/res_path_tokenizer.h"


namespace azer {
namespace files {
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
    return ResPathTokenizer::kNoTokens;
  }

  bool demils = false;
  current_.clear();
  while (c) {
    switch (c) {
      case FILE_PATH_LITERAL(':'): 
      case FILE_PATH_LITERAL('/'): 
      case FILE_PATH_LITERAL('.'): 
        demils = true;
        if (!current_.empty() && prev != c) {
          return ResPathTokenizer::kSuccess;
        } else {
		  token_type_ = static_cast<Type>(c);
          current_.push_back(c);
        }
        break;
      default:
        if (demils) return ResPathTokenizer::kSuccess;
		token_type_ = kStringToken;
        current_.push_back(c);
        if (!ValidStringChar(c)) {
          ++index_;
          return ResPathTokenizer::kContainInvalidChar;
        }
        break;
    }
    prev = c;
    c = *++index_;
  }

  return ResPathTokenizer::kSuccess;
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

ResPathTokenizer::ResPathTokenizer(const StringType& string) 
    : splitter_(string)
    , next_result_(kSuccess) {
}

ResPathTokenizer::~ResPathTokenizer() {
}

int ResPathTokenizer::GetNext() {
  if (next_result_ != kSuccess) { return next_result_;}
  token_.clear();
  if (!next_token_.empty()) {
    token_ = next_token_;
    type_ = next_type_;
    next_token_.clear();
  } else {
    int ret = splitter_.GetNext();
    if (ret == ResPathTokenizer::kNoTokens && !token_.empty()) { 
      return kSuccess;
    } else if (ret != ResPathTokenizer::kSuccess) {
      return ret;
    }

    token_ = splitter_.token();
    type_ = splitter_.token_type();
    if (type_ == ResPathSplitter::kSlashToken) {
      return kSuccess;
    }
  }

  while (true) {
    int ret = splitter_.GetNext();
    if (ret == ResPathTokenizer::kSuccess) {
      next_token_ = splitter_.token();
      next_type_ = splitter_.token_type();
    } else {
      next_result_ = ret;
      return kSuccess;
    }

    if (type_ == ResPathSplitter::kCommaToken
        && next_type_ == ResPathSplitter::kSlashToken) {
      // perhaps proto specifier
      if (next_token_ != "//") {
        return kInvalidComponent;
      }
    } else if (type_ == ResPathSplitter::kSlashToken) {
      return kSuccess;
    } else if (next_type_ == ResPathSplitter::kSlashToken) {
      return kSuccess;
    } else if (next_type_ == ResPathSplitter::kCommaToken) {
      return kSuccess;
    } else {
      token_.append(next_token_);
      next_token_.clear();
      type_ = next_type_;
    }
  }
  
  DCHECK(false);
  return kSuccess;
}
}  // namespace files
}  // namespace azer
