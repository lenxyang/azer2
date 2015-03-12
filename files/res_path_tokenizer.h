#pragma once

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "azer/base/string.h"

namespace azer {
class AZER_EXPORT ResPathTokenizer {
 public:
  explicit ResPathTokenizer(const StringType& string);
  ~ResPathTokenizer();

  enum TokenType {
    kUnkownToken = -1,
    kCommaToken = FILE_PATH_LITERAL(':'),
    kSlashToken = FILE_PATH_LITERAL('/'),
    kDotToken = FILE_PATH_LITERAL('.'),
    kStringToken,
  };

  enum {
    kSuccess = 0,
    kError = 1,
    kNoTokens,
    kContainInvalidChar,
  };
  int GetNext();
  const StringType& token() const { return current_;}
  TokenType token_type() const { return token_type_;}
 private:
  bool ValidStringChar(CharType cb) const;
  const CharType* index_;
  StringType current_;
  StringType raw_;
  TokenType token_type_;
  DISALLOW_COPY_AND_ASSIGN(ResPathTokenizer);
};
}  // namespace azer
