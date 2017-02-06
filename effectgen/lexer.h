#pragma once

#include <string>
#include "base/macros.h"
#include "azer/effectgen/exception.h"
#include "azer/effectgen/token.h"

namespace azer {
namespace effectgen {

class Lexer {
 public:
  explicit Lexer(const std::string& str);

  bool GetNextToken(Token* token);
 private:
  ExceptLoc location() const;
  char peek_char(int index);
  bool GetNextChar();
  bool ParseIdentifier(Token* token);
  void ParseNumber(Token* token);
  void ParseHexNumber(Token* token);
  TokenType lookup_keyword(const std::string& str);
  const std::string data_;
  int index_;
  char last_char_;
  int lineno_;
  int char_offset_;
  std::string file_name_;
  DISALLOW_COPY_AND_ASSIGN(Lexer);
};
}  // namespace effectgen
}  // namespace azer
