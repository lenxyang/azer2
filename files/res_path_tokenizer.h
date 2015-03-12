#pragma once

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "azer/base/string.h"

namespace azer {
class AZER_EXPORT ResPathTokenizer {
 public:
  explicit ResPathTokenizer(const StringType& string);
  ~ResPathTokenizer();

  enum {
    kSuccess = 0,
    kError = 1,
    kNoTokens,
  };
  int GetNext();
  const StringType& token() const { return current_;}

  static CharType kValidCharInPath[];
 private:
  bool ValidStringBeginChar(CharType cb) const;
  bool ValidStringFollowingChar(CharType cb) const;
  const CharType* index_;
  StringType current_;
  StringType raw_;
  DISALLOW_COPY_AND_ASSIGN(ResPathTokenizer);
};
}  // namespace azer
