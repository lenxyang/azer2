#include "azer/effectgen/lexer.h"

#include <algorithm>
#include <cctype>
#include <string>
#include "base/logging.h"
#include "base/strings/stringprintf.h"
#include "base/strings/string_number_conversions.h"
#include "azer/effectgen/exception.h"

namespace azer {
namespace effectgen {
namespace {
struct KeywordMap {
  TokenType type;
  const char* keyword;
};

KeywordMap keyword_map[] = {
  {TokenType::kVertexLayout,    "VertexLayout"},
  {TokenType::kConstantsBuffer, "ConstantsBuffer"},
  {TokenType::kTexture1D,       "Texture1D"},
  {TokenType::kTexture1DArray,  "Texture1DArray"},
  {TokenType::kTexture2D,       "Texture2D"},
  {TokenType::kTexture2DArray,  "Texture2DArray"},
  {TokenType::kTextureCube,     "TextureCube"},
  {TokenType::kTexture3D,       "Texture3D"},
  {TokenType::kSamplerState,    "SamplerState"},
  {TokenType::kStructure,       "struct"},
  {TokenType::kVertexStage,     "vertex"},
  {TokenType::kHullStage,       "hull"},
  {TokenType::kDomainStage,     "domain"},
  {TokenType::kFragmentStage,   "fragment"},
  {TokenType::kGeometryStage,   "geometry"},
  {TokenType::kSemantic,        "semantic"},
  {TokenType::kOffset,          "offset"},
  {TokenType::kStream,          "stream"},
  {TokenType::kNone, NULL},
};


}

using ::base::debug::StackTrace;
using ::base::StringPrintf;

Lexer::Lexer(const std::string& str)
    : data_(str),
      index_(-1),
      last_char_(' '),
      lineno_(0),
      char_offset_(0) {
}

bool Lexer::GetNextToken(Token* token) {
  if (std::isalpha(last_char_) && ParseIdentifier(token)) {
    return true;
  }

  if (last_char_ == '0' && peek_char(1) == 'x') {
    ParseHexNumber(token);
    return true;
  }

  if (std::isdigit(last_char_)) {
    ParseNumber(token);
    return true;
  }

  return false;
}

char Lexer::peek_char(int index) {
  if (index_ + index >= data_.length()) {
    return '\0';
  }
  return *(data_.c_str() + index_ + index);
}

bool Lexer::GetNextChar() {
  if (index_ >= data_.length()) {
    return false;
  }

  char_offset_++;
  char c  = *(data_.c_str() + ++index_);
  if (c == '\n') {
    lineno_++;
    char_offset_ = 0;
  }
  last_char_ = c;
  
  return true;
}

void Lexer::ParseHexNumber(Token* token) {
  CHECK(false);
}

void Lexer::ParseNumber(Token* token) {
  bool float_num = false; 
  while (std::isdigit(last_char_) || last_char_ == '.') {
    if (last_char_ == '.') {
      float_num = true;
    }
    token->data.push_back(last_char_);
    GetNextChar();
  }

  if (float_num) {
    if (!std::isspace(last_char_) || last_char_ != 'f') {
      throw InvalidNumException(
          StringPrintf("Invalid float number %s", token->data.c_str()),
          location(), StackTrace());
    }

    if (!::base::StringToDouble(token->data.c_str(), &token->dval)) {
      throw InvalidNumException(
          StringPrintf("Invalid float number %s", token->data.c_str()),
          location(), StackTrace());
    }
  } else if (std::isspace(last_char_)) {
    if (!::base::StringToInt64(token->data.c_str(), &token->ival)) {
      throw InvalidNumException(
          StringPrintf("Invalid integer number %s", token->data.c_str()),
          location(), StackTrace());
    }
  } else {
    throw InvalidNumException(
        StringPrintf("Invalid supfix for number %s", last_char_, token->data.c_str()),
        location(), StackTrace());
  }
}

ExceptLoc Lexer::location() const {
  ExceptLoc loc;
  loc.file = file_name_;
  loc.lineno = lineno_;
  loc.offset = char_offset_;
  return loc;
}

bool Lexer::ParseIdentifier(Token* token) {
  DCHECK(token->data.empty());
  while (std::isalnum(last_char_)) {
    token->data.push_back(last_char_);
    if (!GetNextChar()) {
      break;
    }
  }
  
  token->type = lookup_keyword(token->data);
  if (token->type != TokenType::kNone) {
    token->type = TokenType::kIdentifier;
  }
  return true;
}

TokenType Lexer::lookup_keyword(const std::string& str) {
  KeywordMap* cur = keyword_map;
  while (cur->keyword != NULL) {
    if (str == cur->keyword) {
      return cur->type;
    }
    cur++;
  }
  return TokenType::kNone;
}
}  // namespace effectgen
}  // namespace azer
