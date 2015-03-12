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
  void Apply(ResPath* respath);
  bool success() const;

  enum State {
    kStart,
    kRootSlashBegin,
    kRootSlashEnd,
    kProtoSlahBegin,
    kProtoSlahEnd,
    kDot,
    kSlash1,
    kSlash2,
    kString,
    kComponent,
    kFailed,
    kFinished,
  };
  State state() const { return state_;}
  void set_state(State state) { state_ = state;}

  const StringType& component() const { return component_;}
  const StringType& proto() const { return proto_;}
  const std::vector<StringType>& dirs() const { return dirs_;}

  static CharType kValidCharInPath[];
  static CharType kValidDelimsInPath[];
 private:
  void SetErrorMsg(const std::string& msg);
  bool HandleToken(const StringType& token);

  bool HandleStartState(const StringType& token);
  std::vector<StringType> dirs_;
  StringType raw_;
  StringType normalized_;
  StringType proto_;
  StringType component_;

  State state_;
  std::string error_msg_;
  DISALLOW_COPY_AND_ASSIGN(ResPathNormalizer);
};
}  // namespace azer
