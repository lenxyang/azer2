#include "azer/render/shader_param_table.h"
#include "azer/base/align.h"

namespace azer {

int32_t ShaderParamTable::offset(int32_t index) const {
  DCHECK_GE(index, 0);
  DCHECK_LT(index, static_cast<int32_t>(constants_.size()));
  return constants_[index].offset;
}

void ShaderParamTable::SetValue(int32_t idx, const void* value, int32_t size) {
  return SetValueWithOffset(idx, 0, value, size);
}

void ShaderParamTable::SetMatrix(int32_t idx, const Matrix4* matrices,
                                  int num) {
  return SetValue(idx, (const void*)matrices, sizeof(Matrix4) * num);
}

void ShaderParamTable::SetData(int offset, const void* value, int32_t size) {
  memcpy(data_.get() + offset, value, size);
  DCHECK_LE(offset + size, this->size());
}

int32_t GpuTableItemDescSize(const ShaderParamTable::Desc& desc) {
  if (desc.type == ShaderParamType::kSelfDefined) {
    DCHECK_NE(desc.element_size, -1);
    return desc.element_size;
  } else {
    return GpuTableItemTypeSize(desc.type);
  }
}

ShaderParamTable::ShaderParamTable(int32_t num, const Desc* desc)
    : GpuResource(kShaderConstsTableBufferOpt(), GpuResType::kConstantTable) {
  uint32_t offset = 0;
  const Desc* curr = desc;
  for (int32_t i = 0; i < num; ++i, ++curr) {
    const int32_t size = GpuTableItemDescSize(*curr);
    const int32_t total_size = size * curr->num;
    DCHECK_LE(offset, curr->offset);
    offset = curr->offset;
    DCHECK(offset != -1);
    constants_.push_back(Variable(*curr, size, total_size, offset));
    offset += total_size;
  }
  size_ = AZER_ALIGN_64(offset);
  data_.reset(new uint8_t[size_]);
}

void ShaderParamTable::SetArrayItem(int32_t idx, int32_t arridx, const void* value,
                                     int32_t size) {
  DCHECK_GT(constants_.size(), 0u);
  DCHECK(idx >= 0 && idx < static_cast<int>(constants_.size()));
  DCHECK(arridx >= 0 && arridx < constants_[idx].desc.num);
  const Variable& variable = constants_[idx];
  DCHECK_EQ(size, variable.element_size);
  SetData(variable.offset + size * arridx, value, size);
}

void ShaderParamTable::SetArrayMultiItem(int32_t idx, int32_t arridx, const void* value,
                                          int32_t size) {
  DCHECK_GT(constants_.size(), 0u);
  DCHECK(idx >= 0 && idx < static_cast<int>(constants_.size()));
  DCHECK(arridx >= 0 && arridx < constants_[idx].desc.num);
  const Variable& variable = constants_[idx];
  DCHECK_LE(variable.element_size * arridx + size, variable.size);
  SetData(variable.offset + size * arridx, value, size);
}

void ShaderParamTable::SetValueWithOffset(int32_t idx, int32_t offset,
                                           const void* value, int32_t size) {
  DCHECK_GT(constants_.size(), 0u);
  DCHECK(idx >= 0 && idx < static_cast<int>(constants_.size()));
  const Variable& variable = constants_[idx];
  DCHECK_LE(size, variable.size);
  SetData(variable.offset + offset, value, size);
}

GpuResLockDataPtr ShaderParamTable::map(MapType flags) {
  NOTIMPLEMENTED();
  return GpuResLockDataPtr();
}
void ShaderParamTable::unmap() {
  NOTIMPLEMENTED();
}

int32_t GpuTableItemTypeSize(const ShaderParamType::Type type) {
  switch(type) {
    case ShaderParamType::kFloat: return sizeof(float);
    case ShaderParamType::kVector2: return sizeof(azer::Vector2);
    case ShaderParamType::kVector3: return sizeof(azer::Vector3);
    case ShaderParamType::kVector4: return sizeof(azer::Vector4);
    case ShaderParamType::kInt: return sizeof(uint32_t);
    case ShaderParamType::kIntVec2: return sizeof(uint32_t) * 2;
    case ShaderParamType::kIntVec3: return sizeof(uint32_t) * 3;
    case ShaderParamType::kIntVec4: return sizeof(uint32_t) * 4;
    case ShaderParamType::kMatrix3: return sizeof(azer::Matrix3);
    case ShaderParamType::kMatrix4: return sizeof(azer::Matrix4);
    case ShaderParamType::kSampler1D: return sizeof(uint32_t);
    case ShaderParamType::kSampler2D: return sizeof(uint32_t);
    case ShaderParamType::kSampler3D: return sizeof(uint32_t);
    case ShaderParamType::kSamplerCube: return sizeof(uint32_t);
    case ShaderParamType::kSelfDefined:
      return -1;
    default:
      CHECK(false) << "No such ShaderParamType: " << (int32_t)type;
      return 0;
  }
}
}  // namespace azer
