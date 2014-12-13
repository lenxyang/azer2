#pragma once

#include <string>
#include <vector>
#include <memory>
#include "base/logging.h"

namespace azer {
namespace afx {

enum StorageQualifier {
  kNoQualifier = 0,
  kIn         = 0x00000001,
  kOut        = 0x00000002,
  kInout      = 0x00000004,
  kConst      = 0x00000008,
  kUniform    = 0x00000010,
};

enum BasicType {
  kTypeNotSpec = -1,
  kVoid = 1,
  kBoolean,
  kChar,
  kShort,
  kUint32,
  kInt32,
  kFloat,
  kDouble,
  kString,
  kVector2,
  kVector3,
  kVector4,
  kIntVec2,
  kIntVec3,
  kIntVec4,
  kMatrix2,
  kMatrix3,
  kMatrix4,
  kIMatrix2,
  kIMatrix3,
  kIMatrix4,
  kTexture1D,
  kTexture1DArray,
  kTexture2D,
  kTexture2DArray,
  kTexture3D,
  kTextureCube,
  kStructure,
  kPointStream,
  kLineStream,
  kLineAdjStream,
  kTriangleStream,
  kTriangleAdjStream,
  kAnonymouseStructure,
  kFunctional,
};

enum Operator {
  kOpNull,           //
  kOpNegative,       // -
  kOpInc,            // ++
  kOpDec,            // --
  kOpLogicalNot,     // !
  kOpBitwiseNot,     // ~

  kOpAdd,            // +
  kOpSub,            // -
  kOpMul,            // *
  kOpDiv,            // /
  kOpMod,            // %
  kOpGe,             // >=
  kOpGt,             // >
  kOpLe,             // <=
  kOpLt,             // <
  kOpNe,             // !=
  kOpEq,             // ==
  kOpLogicalAnd,     // &&
  kOpLogicalOr,      // ||
  kOpLogicalXor,     // ^^
  kOpBitwiseAnd,     // &
  kOpBitwiseOr,      // |
  kOpBitwiseXor,     // ^
  kOpBitwiseLshift,  // <<
  kOpBitwiseRshift,  // >>
  kOpAssign,         // =
  kOpAddAssign,      // +=
  kOpSubAssign,      // -=
  kOpMulAssign,      // *=
  kOpDivAssign,      // \=
  kOpModAssign,      // %=
  kOpAndAssign,      // &=
  kOpOrAssign,       // |=
  kOpXorAssign,      // ^=
  kOpLShiftAssign,   // <<=
  kOpRShiftAssign,   // >>=
  kOpIndex,          // []
  kOpMember,         // .
};

const char* type_name(BasicType type);
const char* qualifier_name(StorageQualifier qualifier);
const char* operator_str(Operator op);

class ASTNode;
class StructDeclNode;

class Type;
typedef std::shared_ptr<Type> TypePtr;

/**
 * Note: Type 如果保存的是 struct name, 那么他永远都应该使用完整名称
 * 即：带有 package 前缀
 */
class Type {
 public:
  Type();
  Type(BasicType type);
  Type(BasicType type, StorageQualifier q);

  bool IsStructure() const { return type_ == kStructure;}
  bool IsAnomyousStruct() const { return struct_name_.empty();}
  bool IsBasicType() const;
  bool IsTexture() const;
  bool IsArray() const;
  bool IsVector() const;
  bool IsIntVec() const;
  bool IsMatrix() const;
  bool IsScalar() const;
  bool IsStream() const;

  void SetStructName(const std::string& name);
  void AppendTemplateArg(TypePtr& ptr);
  TypePtr& GetTemplateArg(int index);
  const TypePtr& GetTemplateArg(int index) const;
  std::vector<TypePtr>& GetTemplateArgs() { return template_args_;}
  const std::vector<TypePtr>& GetTemplateArgs() const { return template_args_;}

  const std::string& struct_name() const { return struct_name_;}

  void SetType(BasicType t);
  BasicType type() const { return type_;}
  void SetStorageQualifier(StorageQualifier q) { storage_qualifier_ = q; }
  StorageQualifier storage_qualifier() const { return storage_qualifier_; }
  const std::vector<int>& GetDimVec() const { return dim_;}
  void PushDim(int dim) { dim_.push_back(dim); }
  void SetDim(int index, int dim);
  int GetDim(int index) const;
 private:
  StorageQualifier storage_qualifier_;
  BasicType type_;

  /** 
   * 用于表示 structure 的名称
   */
  std::string struct_name_;
  /**
   * 用于表示 template 的名称
   */
  std::vector<TypePtr> template_args_;
  std::vector<int> dim_;
  DISALLOW_COPY_AND_ASSIGN(Type);
};

inline void Type::SetType(BasicType t) {
  DCHECK_EQ(type_, kTypeNotSpec);
  type_ = t;
}

inline bool Type::IsBasicType() const {
  return type() >= kVoid && type() < kMatrix4;
}

inline bool Type::IsTexture() const {
  if (type() >= kTexture1D && type() <= kTextureCube) { return true; }
  else { return false; }
}
inline bool Type::IsArray() const {
  return dim_.size() != 0;
}

inline bool Type::IsIntVec() const {
  return type() == kIntVec2 || type() == kIntVec3 || type() == kIntVec4;
}

inline bool Type::IsVector() const {
  return type() == kVector2 || type() == kVector3 || type() == kVector4;
}

inline bool Type::IsMatrix() const {
  return type() == kMatrix2 || type() == kMatrix3 || type() == kMatrix4;
}

inline bool Type::IsScalar() const {
  return !IsMatrix() && !IsVector() && !IsIntVec()
      && !IsArray() && !IsTexture() && !IsStructure();
}

inline bool Type::IsStream() const {
  return type() == kPointStream
      || type() == kLineStream
      || type() == kLineAdjStream
      || type() == kTriangleStream
      || type() == kTriangleAdjStream;
}

inline void Type::SetStructName(const std::string& name) {
  struct_name_ = name;
}

inline void Type::AppendTemplateArg(TypePtr& ptr) {
  template_args_.push_back(ptr);
}

inline TypePtr& Type::GetTemplateArg(int index) {
  CHECK_LT(index, static_cast<int32>(template_args_.size()));
  CHECK_GT(index, 0);
  return template_args_[index];
}

inline const TypePtr& Type::GetTemplateArg(int index) const {
  CHECK_LT(index, static_cast<int32>(template_args_.size()));
  CHECK_GT(index, 0);
  return template_args_[index];
}

inline void Type::SetDim(int index, int dim) {
  DCHECK_LT(index, (int)dim_.size());
  dim_[index] = dim;
}

inline int Type::GetDim(int index) const {
  DCHECK_LT(index, (int)dim_.size());
  return dim_[index];
}

uint32 SizeofType(const Type& type);

inline bool IsTypeSupportMemberOper(const TypePtr& type) {
  // float vector support swizzle expression
  return type->IsVector() || type->IsStructure();
}

inline bool IsTypeStream(const TypePtr& type) {
  switch (type->type()) {
    case kPointStream:
    case kLineStream:
    case kLineAdjStream:
    case kTriangleStream:
    case kTriangleAdjStream: 
      return true;
    default:
      return false;
  }
}

inline bool IsIntegerScalar(const TypePtr& type) {
  if (type->IsArray()) return false;
  return type->type() == kInt32
      || type->type() == kUint32
      || type->type() == kShort;
}

}  // namespace afx
}  // namespace azer
