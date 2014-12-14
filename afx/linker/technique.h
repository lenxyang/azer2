#pragma once

#include <string>
#include <vector>

#include "azer/afx/compiler/astnode.h"
#include "azer/render/render_system_enum.h"

namespace azer {
namespace afx {

struct Technique {
  struct StageInfo {
    ASTNode* entry;
    ASTNodeVec depend;
    ASTNodeVec uniforms;
    ASTNodeVec uni_depend;
    ASTNodeVec uni_textures;
    StageInfo(): entry(NULL) {}
  };

  std::string name;
  std::vector<StageInfo> shader;
  AttributesNode* attributes;
};

}  // namespace afx
}  // namespace azer
