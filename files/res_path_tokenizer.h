#pragma once

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "azer/base/string.h"

namespace azer {
namespace files {
class AZER_EXPORT ResPathSplitter {
 public:
  explicit ResPathSplitter(const StringType& string);
  ~ResPathSplitter();

  enum Type {
    kUnkownToken = -1,
    kStringToken = 1,
    kCommaToken = FILE_PATH_LITERAL(':'),
    kSlashToken = FILE_PATH_LITERAL('/'),
    kDotToken = FILE_PATH_LITERAL('.'),
  };

  int GetNext();
  const StringType& token() const { return current_;}
  Type token_type() const { return token_type_;}
 private:
  bool ValidStringChar(CharType cb) const;
  const CharType* index_;
  StringType current_;
  StringType raw_;
  Type token_type_;
  DISALLOW_COPY_AND_ASSIGN(ResPathSplitter);
};

class AZER_EXPORT ResPathTokenizer {
 public:
  explicit ResPathTokenizer(const StringType& string);
  ~ResPathTokenizer();

  enum {
    kSuccess = 0,
    kError = 1,
    kNoTokens,
    kContainInvalidChar,
    kInvalidComponent,
  };

  enum TokenType {
    kProtoSpcecifier,
    kComponent,
    kRoot,
    kDots,
    kDirSplitter,
    kName,
  };
  int GetNext();
  const StringType& token() const { return token_;}
  TokenType type() const { return token_type_;}
 private:
  bool Init();

  struct Token {
    ResPathSplitter::Type type;
    StringType token;
  };
  int HandleCommaToken(const Token& token);
  int HandleSlashToken(const Token& token);
  int HandleDotToken(const Token& token);
  int HandleStringToken(const Token& token);
  int HandleStringTokenWithProtoProbility(const Token& token);

  const Token& GetSplitterToken(int index) const;
  
  StringType string_;
  StringType token_;
  
  std::vector<Token> token_list_;
  int index_;
  TokenType token_type_;
  DISALLOW_COPY_AND_ASSIGN(ResPathTokenizer);
};
}  // namespace files
}  // namespace azer
