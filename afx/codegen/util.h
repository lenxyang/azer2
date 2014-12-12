#pragma once

#include <string>
#include <vector>

namespace azer {
namespace afx {

class ASTNode;

/**
 * 获取访问路径
 * a.b.tex 那么他将被分成一个 std::vector<std::string> = {a, b, tex}
 */
void GetTexRefferPath(ASTNode* node, std::vector<std::string>* path);
std::string GenReferredTextureVarName(ASTNode* node);

}  // namespace afx
}  // namespace azer
