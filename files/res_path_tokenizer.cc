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
    : string_(string) {
}

ResPathTokenizer::~ResPathTokenizer() {
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

  return ret == kNoTokens;
}

int ResPathTokenizer::HandleCommaToken(const Token& token) {
  token_.append(token.token);
  const Token& next_token = GetSplitterToken(index_++);
  if (next_token.type == ResPathSplitter::kStringToken) {
    return HandleStringToken(next_token);
  } else if (next_token.type == ResPathSplitter::kDotToken) {
    return HandleStringToken(next_token);
  } else {
  }
}

int ResPathTokenizer::HandleSlashToken(const Token& token) {
  token_.append(token.token);
  return kSuccess;
}

int ResPathTokenizer::HandleDotToken(const Token& token) {
  token_.append(token.token);
  const Token& next_token = GetSplitterToken(++index_);
  if (next_token == ResPathSplitter::kStringToken) {
    return HandleStringToken(next_token);
  } else {
    return ;
  }
}

int ResPathTokenizer::HandleStringToken(const Token& token) {
  token_.append(token.token);
  const Token& next_token = GetSplitterToken(++index_);
  if (next_token == ResPathSplitter::kDotToken) {
    return HandleDotToken(next_token);
  } else {
    return ;
  }
}

int ResPathTokenizer::HandleStringTokenWithProtoProbility(const Token& token) {
}

int ResPathTokenizer::GetNext() {
  token_.clear();
  const Token& token = GetSplitterToken(index_);
  if (token.type == ResPathSplitter::kCommaToken) {
    return HandleCommaToken(token);
  } else if (token.type == ResPathSplitter::kSlashToken) {
    return HandleSlashToken(token);
  } else if (token.type == ResPathSplitter::kDotToken) {
    return HandleDotToken(token);
  } else if (token.type == ResPathSplitter::kStringToken) {
    return HandleStringTokenWithProtoProbility(token);
  } else {
    return kSuccess;
  }
}

const Token& ResPathTokenizer::GetSplitterToken(int index) const {
  return token_list_[index];
}
}  // namespace files
}  // namespace azer
