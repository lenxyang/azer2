#pragma once

#include <string>

#include "base/basictypes.h"
#include "azer/base/string.h"
#include "azer/files/res_path.h"

namespace azer {
class AZER_EXPORT ResPathNormalizer {
 public:
  explicit ResPathNormalizer(const StringType& path);

  void Normalize();

  const StringType& normalized() const;
  bool success() const;

  enum State {
    kStart,
    kRootSlashBegin,
    kRootSlashEnd,
    kComma,
    kProtoSlahBegin,
    kProtoSlahEnd,
    kNameDot,
    kDot1,
    kDot2,
    kSlash1,
    kSlash2,
    kString,
    kFailed,
    kFinished,
  };
  State state() const { return state_;}
  void set_state(State state) { state_ = state;}

  const StringType& component() const { return component_;}
  const StringType& proto() const { return proto_;}
  const std::vector<StringType>& dirs() const { return dirs_;}
 private:
  void SetErrorMsg(const std::string& msg);
  std::vector<StringType> dirs_;
  StringType raw_;
  StringType normalized_;
  StringType proto_;
  StringType component_;

  State state_;
  std::string error_msg_;
  static CharType kValidCharInPath[];
  DISALLOW_COPY_AND_ASSIGN(ResPathNormalizer);
};
}  // namespace azer
