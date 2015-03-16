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
    return ResPathParser::kNoTokens;
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
          return ResPathParser::kSuccess;
        } else {
		  token_type_ = static_cast<Type>(c);
          current_.push_back(c);
        }
        break;
      default:
        if (demils) return ResPathParser::kSuccess;
		token_type_ = kStringToken;
        current_.push_back(c);
        if (!ValidStringChar(c)) {
          ++index_;
          return ResPathParser::kContainInvalidChar;
        }
        break;
    }
    prev = c;
    c = *++index_;
  }

  return ResPathParser::kSuccess;
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

ResPathParser::ResPathParser(const StringType& string) 
    : string_(string)
    , index_(0)
    , following_token_(false) {
}

ResPathParser::~ResPathParser() {
}

void ResPathParser::SetTokenTypeIfNotSpecified(TokenType type) {
  DCHECK_NE(type, kNotSpecified);
  if (token_type_ != kNotSpecified) {
    token_type_ = type;
  }
}

bool ResPathParser::Init() {
  ResPathSplitter splitter(string_);
  int ret = kSuccess;
  Token token;
  while ((ret = splitter.GetNext()) == kSuccess) {
    token.token = splitter.token();
    token.type = splitter.token_type();
    token_list_.push_back(token);
  }

  token.type = ResPathSplitter::kEndToken;
  token.token.clear();
  token_list_.push_back(token);
  return ret == kNoTokens;
}

int ResPathParser::HandleCommaToken(const Token& token) {
  token_.append(token.token);
  if (token_.size() > 1u) {
    return kUnknownFormat;
  }

  token_type_ = kComponent;
  const Token& next_token = GetSplitterToken(index_);
  if (next_token.type == ResPathSplitter::kStringToken) {
    index_++;
    return HandleStringToken(next_token);
  } else if (next_token.type == ResPathSplitter::kDotToken) {
    if (GetSplitterToken(index_ + 1).type == ResPathSplitter::kStringToken) {
      index_++;
      return HandleDotToken(next_token);
    } else {
      SetTokenTypeIfNotSpecified(kErrorToken);
      return kInvalidComponent;
    }
  } else {
    return kSuccess;
  }
}

int ResPathParser::HandleSlashToken(const Token& token) {
  token_.append(token.token);
  if (!following_token_) {
    if (token_.size() == 2u) {
      token_type_ = kRoot;
      return kSuccess;
    } else {
      SetTokenTypeIfNotSpecified(kErrorToken);
      return kInvalidRoot;
    }
  } else {
    SetTokenTypeIfNotSpecified(kDirSplitter);
    return kSuccess;
  }
}

int ResPathParser::HandleDotTokenWithNameProbility(const Token& token) {
  const Token& next_token = GetSplitterToken(index_);
  if (next_token.type == ResPathSplitter::kSlashToken
      || next_token.type == ResPathSplitter::kCommaToken) {
    if (token.token.length() == 1u) {
      SetTokenTypeIfNotSpecified(kCurrentDir);
      return kSuccess;
    } else if (token.token.length() == 2u) {
      SetTokenTypeIfNotSpecified(kPrevDir);
      return kSuccess;
    } else {
      SetTokenTypeIfNotSpecified(kErrorToken);
      return kError;
    }
  } else {
    return HandleDotToken(token);
  }
}

int ResPathParser::HandleDotToken(const Token& token) {
  SetTokenTypeIfNotSpecified(kName);
  token_.append(token.token);
  const Token& next_token = GetSplitterToken(index_);
  if (next_token.type == ResPathSplitter::kStringToken) {
    index_++;
    return HandleStringToken(next_token);
  } else {
    return kSuccess;
  }
}

int ResPathParser::HandleStringToken(const Token& token) {
  SetTokenTypeIfNotSpecified(kName);
  token_.append(token.token);
  const Token& next_token = GetSplitterToken(index_);
  if (next_token.type == ResPathSplitter::kDotToken) {
    index_++;
    return HandleDotToken(next_token);
  } else {
    return kSuccess;
  }
}

int ResPathParser::HandleStringTokenWithProtoProbility(const Token& token) {
  if (GetSplitterToken(index_).type == ResPathSplitter::kCommaToken
      && GetSplitterToken(index_).token.length() == 1u
      && GetSplitterToken(index_ + 1).type == ResPathSplitter::kSlashToken
      && GetSplitterToken(index_ + 1).token.length() == 2u) {
    token_.append(token.token);
    index_ += 2;
    token_.append(FILE_PATH_LITERAL("://"));
    SetTokenTypeIfNotSpecified(kProtoSpcecifier);
    return kSuccess;
  } else {
    return HandleStringToken(token);
  }
}

int ResPathParser::GetNext() {
  if (!following_token_) {
    if (!Init()) {
      return kError;
    }
  }

  int ret = kError;
  token_.clear();
  token_type_ = kNotSpecified;
  const Token& token = GetSplitterToken(index_++);
  if (token.type == ResPathSplitter::kCommaToken) {
    ret = HandleCommaToken(token);
  } else if (token.type == ResPathSplitter::kSlashToken) {
    ret = HandleSlashToken(token);
  } else if (token.type == ResPathSplitter::kDotToken) {
    ret = HandleDotTokenWithNameProbility(token);
  } else if (token.type == ResPathSplitter::kStringToken) {
    if (following_token_) {
      ret = HandleStringToken(token);
    } else {
      ret = HandleStringTokenWithProtoProbility(token);
    }
  } else if (token.type == ResPathSplitter::kEndToken) {
    token_type_ = kEnd;
    token_ = token.token;
    ret = kSuccess;
    index_--;
  } else {
    ret = kUnknownFormat;
  }

  following_token_ = true;
  return ret;
}

const ResPathParser::Token& ResPathParser::GetSplitterToken(int index) const {
  return token_list_[index];
}
}  // namespace files
}  // namespace azer
