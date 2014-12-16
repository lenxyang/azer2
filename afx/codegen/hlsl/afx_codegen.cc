#include "azer/afx/codegen/hlsl/afx_codegen.h"

#include <sstream>

#include "base/logging.h"
#include "azer/base/string.h"
#include "azer/afx/codegen/ast_code_generator.h"
#include "azer/afx/codegen/hlsl/util.h"
#include "azer/afx/codegen/hlsl/tex_util.h"
#include "azer/afx/linker/attribute_name.h"
#include "azer/afx/compiler/astnode.h"
#include "azer/afx/compiler/astutil.h"
#include "azer/afx/compiler/debug.h"
#include "azer/afx/compiler/afxl.h"
#include "base/files/file_path.h"

namespace azer {
namespace afx {

namespace {
bool IsConstBufferMember(ASTNode* node) {
  DCHECK(node->IsDeclarationNode());
  DeclarationNode* decl = node->ToDeclarationNode();
  TypePtr typeptr = decl->GetType();
  if (typeptr->IsStructure()) {
    StructDeclNode* struct_decl = decl->GetTypedNode()->GetStructDecl();
    DCHECK(NULL != struct_decl);
    return !HasOnlyTextureField(struct_decl);
  } else {
    return true;
  }
}
}  // namespace

HLSLAfxCodegen::HLSLAfxCodegen()  {
}

HLSLAfxCodegen::~HLSLAfxCodegen() {
  delete factory_;
}

std::string HLSLAfxCodegen::GenCode(const Technique::StageInfo& shader,
                                      bool comments) {
  comments_ = comments;
  std::vector<ASTNode*> uniforms;
  std::stringstream ss;

  if (comments_) {
    ss << "/**\n"
       << " * code generated by afxc, donnot modify by hand\n"
       << " * afx code if from file: \n"
       << " *   " << shader.entry->GetContext()->path().value() << "\n"
       << " */\n\n";
  }

  
  ss << "// using row_major\n"
     << "#pragma pack_matrix(row_major)\n\n";
  ss << std::move(GenDeps(shader));
  ss << std::move(GenUniformDeps(shader));
  ss << std::move(GenUniform(shader.uniforms));
  ss << std::move(GenTextureDecl(shader.uni_textures));
  ss << std::move(GenEntry(shader.entry));
  return ss.str();
}


std::string HLSLAfxCodegen::GenDeps(const Technique::StageInfo& shader) {
  std::stringstream ss;
  for (auto iter = shader.depend.rbegin(); iter != shader.depend.rend(); ++iter) {
    ASTNode* node = *iter;
    ss << std::move(GenDepend(node));
  }

  return ss.str();
}

std::string HLSLAfxCodegen::GenUniformDeps(const Technique::StageInfo& shader) {
  std::stringstream ss;
  for (auto iter = shader.uni_depend.rbegin();
       iter != shader.uni_depend.rend(); ++iter) {
    ASTNode* node = *iter;
    ss << std::move(GenUniDepend(node));
  }

  return ss.str();
}

std::string HLSLAfxCodegen::GenEntry(ASTNode* node) {
  std::stringstream ss;
  ss << GenFuncProto(node) << GenFuncBody(node);
  return ss.str();
}

std::string HLSLAfxCodegen::GenFuncProto(ASTNode* node) {
  DCHECK(node->IsFuncDefNode());
  ASTNode* child = node->first_child();
  while (child) {
    if (child->IsFuncProtoNode()) {
      SnippetCodeGenerator generator(factory_);
      generator.GenCode(child);
      return std::move(generator.GetCode());
    }
    child = child->next_sibling();
  }
  NOTREACHED();
  return "";
}

std::string HLSLAfxCodegen::GenFuncBody(ASTNode* node) {
  DCHECK(node->IsFuncDefNode());
  ASTNode* child = node->first_child();
  while (child) {
    if (child->IsScopedNode()) {
      SnippetCodeGenerator generator(factory_);
      generator.GenCode(child);
      return std::move(generator.GetCode());
    }
    child = child->next_sibling();
  }
  NOTREACHED();
  return "";
}

std::string HLSLAfxCodegen::GenUniform(const std::vector<ASTNode*> &uniforms) {
  std::stringstream ss;
  // sort uniforms
  if (uniforms.size() == 0u) return "";
  ss << "cbuffer c_buffer {";
  for (auto iter = uniforms.begin(); iter != uniforms.end(); ++iter) {
    DCHECK((*iter)->IsDeclarationNode());
    DeclarationNode* decl = (*iter)->ToDeclarationNode();
    if (!IsConstBufferMember(decl)) {
      continue;
    }
    DCHECK (!decl->GetType()->IsTexture());
    SnippetCodeGenerator generator(factory_);
    generator.GenCode(decl);
    ss << std::move(generator.GetCode());
  }
  ss << "};";
  return ss.str();
}

std::string HLSLAfxCodegen::GenDepend(ASTNode* node) {
  std::stringstream ss;
  if (comments_) {
    ss << "// genereated code from for \"" << ASTNodeName(node->type()) << "\"\n"
       << "// which defined in file:\n"
       << "//   " << node->GetContext()->path().value() << "\n";
  }
  SnippetCodeGenerator generator(factory_);
  generator.GenCode(node);
  ss << std::move(generator.GetCode());
  if (node->IsStructDeclNode()) {
    type_depends_.insert(StructFullName(node));
  }
  return ss.str();
}

std::string HLSLAfxCodegen::GenUniDepend(ASTNode* node) {
  if (HasOnlyTextureField(node)) {
    return "";
  }

  if (node->IsStructDeclNode()
      && type_depends_.find(StructFullName(node)) != type_depends_.end()) {
       return "";
  }

  std::stringstream ss;
  if (comments_) {
    ss << "// genereated code from for \"" << ASTNodeName(node->type()) << "\"\n"
       << "// which defined in file:\n"
       << "//   " << node->GetContext()->path().value() << "\n";
  }
  SnippetCodeGenerator generator(factory_);
  generator.GenCode(node);
  ss << std::move(generator.GetCode());
  return ss.str();
}

HLSLVSAfxCodegen::HLSLVSAfxCodegen() {
  factory_ = new HLSLCodeGeneratorFactory;
}

std::string HLSLVSAfxCodegen::GenTextureDecl(const std::vector<ASTNode*>& textures) {
  std::stringstream ss;
  for (auto iter = textures.begin(); iter != textures.end(); ++iter) {
    ASTNode* node = *iter;
    ss << HLSLTextureTypeName(node) << " "
       << HLSLDeclaredTextureFullName(node) << ";\n";
  }
  return ss.str();
}

// class HLSLPSAfxCodegen
HLSLPSAfxCodegen::HLSLPSAfxCodegen() {
  factory_ = new HLSLPSCodeGeneratorFactory;
}

std::string HLSLPSAfxCodegen::GenTextureDecl(const std::vector<ASTNode*>& textures) {
  std::stringstream ss;
  for (auto iter = textures.begin(); iter != textures.end(); ++iter) {
    ASTNode* node = *iter;
    ss << HLSLTextureTypeName(node) << " "
       << HLSLDeclaredTextureFullName(node) << ";\n"
       << "SamplerState " << HLSLUniformTextureSamplerDeclFullName(node) << ";\n";
  }
  return ss.str();
}

// class HLSLGSAfxCodegen
HLSLGSAfxCodegen::HLSLGSAfxCodegen() {
  factory_ = new HLSLGSCodeGeneratorFactory;
}

std::string HLSLGSAfxCodegen::GenTextureDecl(const std::vector<ASTNode*>& textures) {
  std::stringstream ss;
  for (auto iter = textures.begin(); iter != textures.end(); ++iter) {
    ASTNode* node = *iter;
    ss << HLSLTextureTypeName(node) << " "
       << HLSLDeclaredTextureFullName(node) << ";\n";
  }
  return "";
}

std::string HLSLGSAfxCodegen::GenEntry(ASTNode* node) {
  std::stringstream ss;
  AttributesNode* attr = node->attributes();
  attr->Add(AttrNames::kGeometryShaderEntry, "true");
  CHECK (attr && attr->HasAttr(AttrNames::kGSMaxVertexCount));
  ss << "[maxvertexcount(" << attr->GetAttrValue(AttrNames::kGSMaxVertexCount) 
     <<")]" << std::endl;
  ss << GenFuncProto(node) << GenFuncBody(node);
  LOG(ERROR) << ss.str();
  return ss.str();
}
}  // namespace afx
}  // namespace azer
