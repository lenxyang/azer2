#pragma once

#include <string>

namespace azer {
namespace afx {

class AttrNames {
 public:
  /**
   * 在 geometry shader 当中指定最多可以生成多少定点
   */
  static const std::string kGSMaxVertexCount;
  static const std::string kGSVertexType;
  static const std::string kGSPrimitiveType;
  static const std::string kGSEmitVariable;

  static const std::string kName;
  static const std::string kVertexShaderEntry;
  static const std::string kHullShaderEntry;
  static const std::string kDomainShaderEntry;
  static const std::string kGeometryShaderEntry;
  static const std::string kPixelShaderEntry;

  static bool ValidGSPrimitiveTypeValue(const std::string& val);
};

}  // namespace afx
}  // namespace azer
