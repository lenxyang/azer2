#include "azer/render/shader_param_table.h"

#include "azer/base/align.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"
#include "azer/render/gpu_buffer.h"

namespace azer {
ShaderParamTable::Desc::Desc(const char* n, ShaderParamType::Type t, 
                             int32_t off, int elenum)
    : type(t), element_size(-1),  num(elenum), offset(off) {
  if (n)
    strncpy(name, n, sizeof(name) - 1);
  element_size = GpuTableItemTypeSize(t);
}

ShaderParamTable::Desc::Desc(const char* n, int32_t off, int size, int elenum)
    : type(ShaderParamType::kSelfDefined)
    , element_size(size), num(elenum), offset(off) {
  if (name)
    strncpy(name, n, sizeof(name) - 1);
}

ShaderParamTable::Desc::Desc(const Desc& desc) {
  if (desc.name)
    strncpy(this->name, desc.name, sizeof(name));
  this->type = desc.type;
  this->element_size = desc.element_size;
  this->num = desc.num;
  this->offset = desc.offset;
}

ShaderParamTable::ShaderParamTable(const Desc* desc) {
  uint32_t offset = 0;
  const Desc* curr = desc;
  while (curr->type != ShaderParamType::kLastGuard) {
    const int32_t size = GpuTableItemDescSize(*curr);
    const int32_t total_size = size * curr->num;
    DCHECK_LE(offset, curr->offset);
    offset = curr->offset;
    DCHECK(offset != -1);
    constants_.push_back(Variable(*curr, size, total_size, offset));
    offset += total_size;
    ++curr;
  }
  size_ = AZER_ALIGN_8(offset);
  data_.reset(new uint8_t[size_]);

  RenderSystem* rs = RenderSystem::Current();
  CHECK(rs) << "RenderSystem Not Initialized";
  static GpuResOptions resopt;
  resopt.target = kBindTargetContantBuffer;
  resopt.type = GpuResType::kConstantTable;
  gpu_buffer_ = rs->CreateBuffer(resopt, 1, size_, NULL);
  CHECK(gpu_buffer_.get()) << "Initializer GpuBuffer for ShaderParam Failed";
}

ShaderParamTable::ShaderParamTable(int32_t num, const Desc* desc) {
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
  size_ = AZER_ALIGN_8(offset);
  data_.reset(new uint8_t[size_]);

  RenderSystem* rs = RenderSystem::Current();
  CHECK(rs) << "RenderSystem Not Initialized";
  static GpuResOptions resopt;
  resopt.target = kBindTargetContantBuffer;
  resopt.type = GpuResType::kConstantTable;
  gpu_buffer_ = rs->CreateBuffer(resopt, 1, size_, NULL);
  CHECK(gpu_buffer_.get()) << "Initializer GpuBuffer for ShaderParam Failed";
}

void ShaderParamTable::flush(Renderer* renderer) {
  DCHECK(gpu_buffer_.get()) << "GpuBuffer cannot be NULL";
  gpu_buffer_->UpdateData(data_.get(), size_);
}

int32_t ShaderParamTable::offset(int32_t index) const {
  DCHECK_GE(index, 0);
  DCHECK_LT(index, static_cast<int32_t>(constants_.size()));
  return constants_[index].offset;
}

void ShaderParamTable::SetValue(int32_t idx, const void* value, int32_t size) {
  return SetValueWithOffset(idx, 0, value, size);
}

void ShaderParamTable::SetWholeData(const void* data, int32_t size) {
  CHECK_EQ(size, size_);
  memcpy(data_.get(), data, size);
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
    case ShaderParamType::kLastGuard:
      return -1;
    default:
      CHECK(false) << "No such ShaderParamType: " << (int32_t)type;
      return 0;
  }
}
}  // namespace azer
