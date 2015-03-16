#include "azer/files/res_path_tokenizer.h"


namespace azer {
namespace files {
ResPathSplitter::ResPathSplitter(const StringType& string) 
    : raw_(string)
    , following_token_(false) {
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
    : string_(string) {
}

ResPathTokenizer::~ResPathTokenizer() {
}

void SetTokenTypeIfNotSpecified(TokenType type) {
  DCHECK_NE(type, kNotSpecified);
  if (token_type_ != kNotSpecified) {
    token_type_ = type;
  }
}

bool ResPathTokenizer::Init() {
  ResPathSplitter splitter(string_);
  int ret = kSuccess;
  Token token;
  while ((ret = splitter.GetNext()) == kSuccess) {
    token.token = splitter.token();
    token.type = splitter.token_type();
    token_list_.push_back(token);
  }

  token.type = kEndToken;
  token.token.clear();
  token_list_.push_back(token);
  return ret == kNoTokens;
}

int ResPathTokenizer::HandleCommaToken(const Token& token) {
  token_.append(token.token);
  if (token.size() > 1u) {
    return kUnknownFormat;
  }

  token_type_ = kComponent;
  const Token& next_token = GetSplitterToken(index_++);
  if (next_token.type == ResPathSplitter::kStringToken) {
    return HandleStringToken(next_token);
  } else if (next_token.type == ResPathSplitter::kDotToken) {
    return HandleStringToken(next_token);
  } else if (next_token.type == ResPathSplitter::kDotToken) {
    return kSuccess;
  }
}

int ResPathTokenizer::HandleSlashToken(const Token& token) {
  SetTokenTypeIfNotSpecified(kDirSplitter);
  token_.append(token.token);
  return kSuccess;
}

int ResPathTokenizer::HandleDotToken(const Token& token) {
  SetTokenTypeIfNotSpecified(kDots);
  token_.append(token.token);
  const Token& next_token = GetSplitterToken(++index_);
  if (next_token == ResPathSplitter::kStringToken) {
    return HandleStringToken(next_token);
  } else {
    return kSuccess;
  }
}

int ResPathTokenizer::HandleStringToken(const Token& token) {
  SetTokenTypeIfNotSpecified(kName);
  token_.append(token.token);
  const Token& next_token = GetSplitterToken(++index_);
  if (next_token == ResPathSplitter::kDotToken) {
    return HandleDotToken(next_token);
  } else {
    return kSuccess;
  }
}

int ResPathTokenizer::HandleStringTokenWithProtoProbility(const Token& token) {
  if (GetSplitterToken(index_ + 1).token_type == ResPathSplitter::kCommaToken
      && GetSplitterToken(index_ + 1).token.length() == 1u
      && GetSplitterToken(index_ + 1).token_type == ResPathSplitter::kSlashToken
      && GetSplitterToken(index_ + 1).token.length() == FILE_PATH_LITERAL("//")) {
    token_.append(token.token);
    index_ += 2;
    token_.append("://");
    SetTokenTypeIfNotSpecified(kProtoSpcecifier);
    return kSuccess;
  } else {
    return HandleStringToken(token);
  }
}

int ResPathTokenizer::GetNext() {
  if (!following_token_) {
    if (!Init()) {
      return kError;
    }
  }

  int ret = kUnknown;
  token_.clear();
  token_type_ = kNotSpecified;
  const Token& token = GetSplitterToken(index_);
  if (token.type == ResPathSplitter::kCommaToken) {
    ret = HandleCommaToken(token);
  } else if (token.type == ResPathSplitter::kSlashToken) {
    token_type_ = kDirSplitter;
    ret = HandleSlashToken(token);
    if (!following_token_) {
      if (token_ == "//") {
        token_type_ = kRoot;
      } else {
        ret = kInvalidRoot;
      }
    }
  } else if (token.type == ResPathSplitter::kDotToken) {
    ret = HandleDotToken(token);
  } else if (token.type == ResPathSplitter::kStringToken) {
    token_type_ = kName;
    if (!following_token_) {
      ret = HandleStringToken(token);
    } else {
      ret = HandleStringTokenWithProtoProbility(token);
    }
  } else if (token.type == k) {
    ret = kEnd;
  } else {
    token_type_ = kName;
    ret = kUnknownFormat;
  }

  following_token_ = true;
  return ret;
}

const Token& ResPathTokenizer::GetSplitterToken(int index) const {
  return token_list_[index];
}
}  // namespace files
}  // namespace azer
