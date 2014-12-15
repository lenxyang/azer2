#pragma once

#include <string>

namespace azer {
namespace afx {

class AttrNames {
 public:
  /**
   * 在 geometry shader 当中指定最多可以生成多少定点
   */
  static const std::string kMaxVertexCount;

  static const std::string kName;
  static const std::string kVertexShaderEntry;
  static const std::string kHullShaderEntry;
  static const std::string kDomainShaderEntry;
  static const std::string kGeometryShaderEntry;
  static const std::string kPixelShaderEntry;
};

}  // namespace afx
}  // namespace azer
