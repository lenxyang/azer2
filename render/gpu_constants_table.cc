#include "azer/render/gpu_constants_table.h"
#include "azer/base/align.h"

namespace azer {

int32_t GpuConstantsTable::offset(int32_t index) const {
  DCHECK_GE(index, 0);
  DCHECK_LT(index, static_cast<int32_t>(constants_.size()));
  return constants_[index].offset;
}

void GpuConstantsTable::SetValue(int32_t idx, const void* value, int32_t size) {
  return SetValueWithOffset(idx, 0, value, size);
}

void GpuConstantsTable::SetMatrix(int32_t idx, const Matrix4* matrices,
                                  int num) {
  return SetValue(idx, (const void*)matrices, sizeof(Matrix4) * num);
}

void GpuConstantsTable::SetData(int offset, const void* value, int32_t size) {
  memcpy(data_.get() + offset, value, size);
  DCHECK_LE(offset + size, this->size());
}

int32_t GpuTableItemDescSize(const GpuConstantsTable::Desc& desc) {
  if (desc.type == GpuConstantsType::kSelfDefined) {
    DCHECK_NE(desc.element_size, -1);
    return desc.element_size;
  } else {
    return GpuTableItemTypeSize(desc.type);
  }
}

GpuConstantsTable::GpuConstantsTable(int32_t num, const Desc* desc)
    : GpuBuffer(kShaderConstsTableBufferOpt()) {
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

void GpuConstantsTable::SetArrayItem(int32_t idx, int32_t arridx, const void* value,
                                     int32_t size) {
  DCHECK_GT(constants_.size(), 0u);
  DCHECK(idx >= 0 && idx < static_cast<int>(constants_.size()));
  DCHECK(arridx >= 0 && arridx < constants_[idx].desc.num);
  const Variable& variable = constants_[idx];
  DCHECK_EQ(size, variable.element_size);
  SetData(variable.offset + size * arridx, value, size);
}

void GpuConstantsTable::SetArrayMultiItem(int32_t idx, int32_t arridx, const void* value,
                                          int32_t size) {
  DCHECK_GT(constants_.size(), 0u);
  DCHECK(idx >= 0 && idx < static_cast<int>(constants_.size()));
  DCHECK(arridx >= 0 && arridx < constants_[idx].desc.num);
  const Variable& variable = constants_[idx];
  DCHECK_LE(variable.element_size * arridx + size, variable.size);
  SetData(variable.offset + size * arridx, value, size);
}

void GpuConstantsTable::SetValueWithOffset(int32_t idx, int32_t offset,
                                           const void* value, int32_t size) {
  DCHECK_GT(constants_.size(), 0u);
  DCHECK(idx >= 0 && idx < static_cast<int>(constants_.size()));
  const Variable& variable = constants_[idx];
  DCHECK_LE(size, variable.size);
  SetData(variable.offset + offset, value, size);
}

GpuBufferDataPtr GpuConstantsTable::map(MapType flags) {
  NOTIMPLEMENTED();
  return GpuBufferDataPtr();
}
void GpuConstantsTable::unmap() {
  NOTIMPLEMENTED();
}

int32_t GpuTableItemTypeSize(const GpuConstantsType::Type type) {
  switch(type) {
    case GpuConstantsType::kFloat: return sizeof(float);
    case GpuConstantsType::kVector2: return sizeof(azer::Vector2);
    case GpuConstantsType::kVector3: return sizeof(azer::Vector3);
    case GpuConstantsType::kVector4: return sizeof(azer::Vector4);
    case GpuConstantsType::kInt: return sizeof(uint32_t);
    case GpuConstantsType::kIntVec2: return sizeof(uint32_t) * 2;
    case GpuConstantsType::kIntVec3: return sizeof(uint32_t) * 3;
    case GpuConstantsType::kIntVec4: return sizeof(uint32_t) * 4;
    case GpuConstantsType::kMatrix3: return sizeof(azer::Matrix3);
    case GpuConstantsType::kMatrix4: return sizeof(azer::Matrix4);
    case GpuConstantsType::kSampler1D: return sizeof(uint32_t);
    case GpuConstantsType::kSampler2D: return sizeof(uint32_t);
    case GpuConstantsType::kSampler3D: return sizeof(uint32_t);
    case GpuConstantsType::kSamplerCube: return sizeof(uint32_t);
    case GpuConstantsType::kSelfDefined:
      return -1;
    default:
      CHECK(false) << "No such GpuConstantType: " << (int32_t)type;
      return 0;
  }
}
}  // namespace azer
