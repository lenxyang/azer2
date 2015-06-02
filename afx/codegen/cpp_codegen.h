#pragma once

#include <string>

#include "base/basictypes.h"
#include "azer/afx/compiler/astnode.h"
#include "azer/afx/linker/technique.h"

namespace azer {
namespace afx {

/**
 * CppCodeGen
 * 生成 C++ 相关的代码
 */
class CppCodeGen {
 public:
  CppCodeGen();
  ~CppCodeGen();

  void GenCode(const Technique& tech);
  const std::string& GetHeadCode() const { return head_code_;}
  const std::string& GetCppCode() const { return cpp_code_;}
 private:
  std::string GenStructDecl(const std::string& name, StructDeclNode* decl) const;
  std::string GetClassName(const Technique& tech) const;
  std::string GetEffectName(const Technique& tech) const;

  /**
   * 生成制定 technique 的相关 c++ 代码
   */
  std::string GenTechnique(const Technique& tech);
  void GenHeadCode(const Technique& tech);
  void GenCppCode(const Technique& tech);

  // generate include directive for external struct declaration 
  std::string GenStructDepIncludeCode(const Technique& tech);

  std::string GenSetMaterial(const Technique& tech);
  std::string GenInit(const Technique& tech);
  std::string GenGpuTableInit(const Technique& tech);
  std::string GenVertexStruct(const Technique& tech);
  std::string GenVertexDesc(const Technique& tech);

  // generate structure of uniform variables
  std::string GenExchangeBuffer(const Technique& tech) const;
  std::string GenStageExchangeBuffer(RenderPipelineStage stage,
                                     const ASTNodeVec& uniforms) const;
  std::string GenStageGpuTable(RenderPipelineStage stage,
                               const ASTNodeVec& uniforms);
  std::string GenStageGpuTableDesc(RenderPipelineStage stage,
                                   const ASTNodeVec& uniforms);
  std::string GenStageUniformStructure(RenderPipelineStage stage,
                                       const ASTNodeVec& uniforms) const;

  // generate function to set uniform variables
  std::string GenUniformFuncs(const Technique& tech);
  std::string GenStageUniforms(RenderPipelineStage stage, const ASTNodeVec& unif);
  std::string GenStageTexture(RenderPipelineStage stage, const ASTNodeVec& unif);
  std::string GenAllTextureMember(const Technique& tech);
  std::string GenStageTextureMember(RenderPipelineStage stage,
                                    const ASTNodeVec& uniforms);

  /**
   * 生成 Effect 的虚函数 UseTexture
   */
  std::string GenUseTexture(const Technique& tech);
  std::string GenTextureStageUse(RenderPipelineStage stage,
                                 const ASTNodeVec& uniforms);

  StructDeclNode* GetVertexDecl(const Technique& tech);
  std::string cpp_code_;
  std::string head_code_;
  DISALLOW_COPY_AND_ASSIGN(CppCodeGen);
};
}  // namespace afx
}  // namespace azer
