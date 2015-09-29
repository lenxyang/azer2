#include "azer/render/util/vertex_pack.h"

#include "base/logging.h"

namespace azer {
int32 memalign(int32 n, int32 align) {
  return (n + align - 1) & (~(align - 1));
}

VertexPack::VertexPack(VertexData* data) 
    : data_(data->pointer()),
      current_(NULL),
      size_(data->buffer_size()),
      unit_size_(data->desc()->stride()),
      kAlignBytes(1) {
  for (int32 i = 0; i < data->desc()->element_num(); ++i) {
    desc_.push_back(data->desc()->descs()[i]);
  }
  calc_offsets();
}

VertexPack::VertexPack(int32 alignbytes, const VertexDesc::Desc* desc,
                       int desc_count, uint8* data, int32 data_size)
    : data_(data),
      current_(NULL),
      size_(data_size),
      unit_size_(0),
      kAlignBytes(alignbytes) {
  for (int32 i = 0; i < desc_count; ++i) {
    desc_.push_back(desc[i]);
  }
  calc_offsets();
}

VertexPack::VertexPack(int32 alignbytes, const std::vector<VertexDesc::Desc>& desc, 
                       uint8* data, int32 data_size) 
    : data_(data),
      current_(NULL),
      size_(data_size),
      unit_size_(0),
      desc_(desc),
      kAlignBytes(alignbytes) {
  calc_offsets();
}

VertexPack::~VertexPack() {
}

int32 VertexPack::index() const {
  return (current_ - data_) / unit_size_;
}

bool VertexPack::first() {
  current_ = data_;
  return current_ < data_ + size_;
}

int32 move(int32 offset) {
  first();
  next(offset);
}

bool VertexPack::next(int32 step) {
  current_ += step * unit_size();
  return current_ < data_ + size_;
}

bool VertexPack::end() {
  return !(current_ < data_ + size_);
}

void VertexPack::calc_offsets() {
  offsets_.clear();
  uint32 offset = 0;
  for (auto iter = desc_.begin(); iter != desc_.end(); ++iter) {
    offset = memalign(offset, kAlignBytes);
    offsets_.push_back(offset);
    offset += SizeofDataFormat(iter->type);
  }

  unit_size_ = memalign(offset, kAlignBytes);
}

void VertexPack::WriteFloat(float v, int32 column) {
  DCHECK(desc_[column].type == kFloat);
  uint8* ptr = current_ + offset_of_column(column);
  *((float*)ptr) = v;
}

void VertexPack::WriteVector2(const Vector2& v, int32 column) {
  uint8* ptr = current_ + offset_of_column(column);
  DCHECK(desc_[column].type == kVec2);
  Vector2* vec = (Vector2*)ptr;
  *vec = v;
}

void VertexPack::WriteVector3(const Vector3& v, int32 column) {
  DCHECK(desc_[column].type == kVec3);
  uint8* ptr = current_ + offset_of_column(column);
  Vector3* vec = (Vector3*)ptr;
  *vec = v;
}

void VertexPack::WriteVector4(const Vector4& v, int32 column) {
  DCHECK(desc_[column].type == kVec4);
  uint8* ptr = current_ + offset_of_column(column);
  Vector4* vec = (Vector4*)ptr;
  *vec = v;
}

int32 VertexPack::offset_of_column(int32 column) const {
  DCHECK(column < offsets_.size());
  return offsets_[column];
}

void VertexPack::ReadFloat(float* v, int32 column) const {
  DCHECK(desc_[column].type == kFloat);
  float* ptr = (float*)(current_ + offset_of_column(column));
  *v = *ptr;
}

void VertexPack::ReadVector2(Vector2* v, int32 column) const {
  DCHECK(desc_[column].type == kVec2);
  Vector2* ptr = (Vector2*)(current_ + offset_of_column(column));
  *v = *ptr;
}

void VertexPack::ReadVector3(Vector3* v, int32 column) const {
  DCHECK(desc_[column].type == kVec3);
  Vector3* ptr = (Vector3*)(current_ + offset_of_column(column));
  *v = *ptr;
}

void VertexPack::ReadVector4(Vector4* v, int32 column) const {
  DCHECK(desc_[column].type == kVec4);
  Vector4* ptr = (Vector4*)(current_ + offset_of_column(column));
  *v = *ptr;
}

int32 VertexPack::unit_size() const {
  return unit_size_;
}

}  // namespace azer
