#include "azer/render/util/vertex_pack.h"

#include "base/logging.h"

namespace azer {
int32 memalign(int32 n, int32 align) {
  return (n + align - 1) & (~(align - 1));
}

VertexPack::VertexPack(VertexData* data) 
    : vertex_data_(data),
      index_(-1),
      kAlignBytes(1) {
}

VertexPack::~VertexPack() {
}

int32 VertexPack::index() const {
  return index_;
}

bool VertexPack::first() {
  index_ = 0;
}

bool VertexPack::move(int32 offset) {
  first();
  index_ = offset;
}

bool VertexPack::next(int32 step) {
  index_ += step;
  return index_ < vertex_data_->vertex_num();
}

bool VertexPack::end() {
  return !(index_ < vertex_data_->vertex_num());
}

void VertexPack::WriteFloat(float v, VertexPos pos) {
  DCHECK(desc_[column].type == kFloat);
  uint8* ptr = current_ + offset_of_column(column);
  *((float*)ptr) = v;
}

void VertexPack::WriteVector2(const Vector2& v, VertexPos pos) {
  uint8* ptr = current_ + offset_of_column(column);
  DCHECK(desc_[column].type == kVec2);
  Vector2* vec = (Vector2*)ptr;
  *vec = v;
}

void VertexPack::WriteVector3(const Vector3& v, VertexPos pos) {
  DCHECK(desc_[column].type == kVec3);
  uint8* ptr = current_ + offset_of_column(column);
  Vector3* vec = (Vector3*)ptr;
  *vec = v;
}

void VertexPack::WriteVector4(const Vector4& v, VertexPos pos) {
  DCHECK(desc_[column].type == kVec4);
  uint8* ptr = current_ + offset_of_column(column);
  Vector4* vec = (Vector4*)ptr;
  *vec = v;
}

int32 VertexPack::offset_of_column(VertexPos pos) const {
  DCHECK(column < offsets_.size());
  return offsets_[column];
}

void VertexPack::ReadFloat(float* v, VertexPos pos) const {
  DCHECK(desc_[column].type == kFloat);
  float* ptr = (float*)(current_ + offset_of_column(column));
  *v = *ptr;
}

void VertexPack::ReadVector2(Vector2* v, VertexPos pos) const {
  DCHECK(desc_[column].type == kVec2);
  Vector2* ptr = (Vector2*)(current_ + offset_of_column(column));
  *v = *ptr;
}

void VertexPack::ReadVector3(Vector3* v, VertexPos pos) const {
  DCHECK(desc_[column].type == kVec3);
  Vector3* ptr = (Vector3*)(current_ + offset_of_column(column));
  *v = *ptr;
}

void VertexPack::ReadVector4(Vector4* v, VertexPos pos) const {
  DCHECK(desc_[column].type == kVec4);
  Vector4* ptr = (Vector4*)(current_ + offset_of_column(column));
  *v = *ptr;
}

VertexData* VertexPack::data() { 
  return vertex_data_.get();
}

VertexDesc* VertexPack::desc() { 
    return vertex_data_->desc();
}
}  // namespace azer
