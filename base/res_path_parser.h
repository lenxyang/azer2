#pragma once


#include "base/strings/string16.h"
#include "azer/base/string.h"

namespace azer {

class ResPath;

namespace files {
class AZER_EXPORT ResPathTokenizer {
 public:
  explicit ResPathTokenizer(const StringType& string);
  ~ResPathTokenizer();

  enum Type {
    kUnkownToken = -1,
    kEndToken = 1,
    kStringToken = 2,
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
  DISALLOW_COPY_AND_ASSIGN(ResPathTokenizer);
};

class AZER_EXPORT ResPathParser {
 public:
  explicit ResPathParser(const StringType& string);
  ~ResPathParser();

  enum {
    kSuccess = 0,
    kError = 1,
    kNoTokens,
    kContainInvalidChar,
    kInvalidComponent,
    kUnknownFormat,
    kTooManyDots,
    kInvalidRoot,
    kTokenFollowingComponent,
  };

  enum Type {
    kEnd = -1,
    kNotSpecified = 0,
    kProtoSpecifier = 1,
    kComponent,
    kRoot,
    kCurrentDir,
    kPrevDir,
    kDirSplitter,
    kName,
    kErrorToken,
  };
  int GetNext();
  const StringType& token() const { return token_;}
  Type type() const { return token_type_;}
 private:
  bool Init();

  struct Token {
    ResPathTokenizer::Type type;
    StringType token;
  };
  int HandleCommaToken(const Token& token);
  int HandleSlashToken(const Token& token);
  int HandleDotToken(const Token& token);
  int HandleDotTokenWithNameProbility(const Token& token);
  int HandleStringToken(const Token& token);
  int HandleStringTokenWithProtoProbility(const Token& token);

  const Token& GetSplitterToken(int index) const;
  
  StringType string_;
  StringType token_;

  void SetTypeIfNotSpecified(Type type);
  void SetType(Type type) { token_type_ = type;}
  std::vector<Token> token_list_;
  bool has_component_;
  int index_;
  Type token_type_;
  bool following_token_;  // not first_token
  DISALLOW_COPY_AND_ASSIGN(ResPathParser);
};

class AZER_EXPORT ResPathNormalizer {
 public:
  explicit ResPathNormalizer(const StringType& raw_path);
  bool Normalize(ResPath* path);
 private:
  StringType proto_;
  StringType component_;
  StringType raw_path_;
  DISALLOW_COPY_AND_ASSIGN(ResPathNormalizer);
};
}  // namespace files
}  // namespace azer
