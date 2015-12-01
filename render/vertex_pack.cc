#include "azer/render/vertex_pack.h"

#include "base/logging.h"
#include "base/strings/string_util.h"

namespace azer {
int32 memalign(int32 n, int32 align) {
  return (n + align - 1) & (~(align - 1));
}

VertexPos::VertexPos() : slot(-1), index(-1) {}
VertexPos::VertexPos(int32 s, int32 i) : slot(s), index(i) {}

VertexPack::VertexPack(VertexData* data) 
    : vertex_data_(data),
      index_(-1),
      kAlignBytes(1) {
}

VertexPack::VertexPack(SlotVertexData* data)
    : index_(-1),
      kAlignBytes(1) {
  vertex_data_ = new VertexData(data->desc(), data->vertex_num());
  vertex_data_->set_slot_vertex_data(SlotVertexDataPtr(data), 0);
}

VertexPack::~VertexPack() {
}

int32 VertexPack::index() const {
  return index_;
}

bool VertexPack::first() {
  index_ = 0;
  return index_ < vertex_data_->vertex_num();
}

bool VertexPack::move(int32 offset) {
  first();
  index_ = offset;
  return index_ < vertex_data_->vertex_num();
}

bool VertexPack::next(int32 step) {
  index_ += step;
  return index_ < vertex_data_->vertex_num();
}

bool VertexPack::end() {
  return !(index_ < vertex_data_->vertex_num());
}

void VertexPack::WriteFloat(float v, const VertexPos& pos) {
  if (pos.slot == -1 || pos.index == -1)
    return;
  DCHECK(get_data_type(pos) == kFloat);
  uint8* ptr = get_data_ptr(pos);
  *((float*)ptr) = v;
}

void VertexPack::WriteVector2(const Vector2& v, const VertexPos& pos) {
  if (pos.slot == -1 || pos.index == -1)
    return;

  uint8* ptr = get_data_ptr(pos);
  DCHECK(get_data_type(pos) == kVec2);
  Vector2* vec = (Vector2*)ptr;
  *vec = v;
}

void VertexPack::WriteVector3(const Vector3& v, const VertexPos& pos) {
  if (pos.slot == -1 || pos.index == -1)
    return;

  DCHECK(get_data_type(pos) == kVec3);
  uint8* ptr = get_data_ptr(pos);
  Vector3* vec = (Vector3*)ptr;
  *vec = v;
}

void VertexPack::WriteVector4(const Vector4& v, const VertexPos& pos) {
  if (pos.slot == -1 || pos.index == -1)
    return;

  DCHECK(get_data_type(pos) == kVec4);
  uint8* ptr = get_data_ptr(pos);
  Vector4* vec = (Vector4*)ptr;
  *vec = v;
}

void VertexPack::WriteVector3Or4(Vector4* v, const VertexPos& pos) const {
  if (pos.slot == -1 || pos.index == -1)
    return;

  if (get_data_type(pos) == kVec4) {
    uint8* ptr = get_data_ptr(pos);
    Vector4* vec = (Vector4*)ptr;
    *vec = *v;
  } else if (get_data_type(pos) == kVec3) {
    uint8* ptr = get_data_ptr(pos);
    Vector3* vec = (Vector3*)ptr;
    vec->x = v->x;
    vec->y = v->y;
    vec->z = v->z;
  } else {
    CHECK(false);
  }
}

void VertexPack::ReadFloat(float* v, const VertexPos& pos) const {
  CHECK(pos.slot != -1 && pos.index != -1);
  DCHECK(get_data_type(pos) == kFloat);
  float* ptr = (float*)(get_data_ptr(pos));
  *v = *ptr;
}

void VertexPack::ReadVector2(Vector2* v, const VertexPos& pos) const {
  CHECK(pos.slot != -1 && pos.index != -1);
  DCHECK(get_data_type(pos) == kVec2);
  Vector2* ptr = (Vector2*)(get_data_ptr(pos));
  *v = *ptr;
}

void VertexPack::ReadVector3(Vector3* v, const VertexPos& pos) const {
  CHECK(pos.slot != -1 && pos.index != -1);
  DCHECK(get_data_type(pos) == kVec3);
  Vector3* ptr = (Vector3*)(get_data_ptr(pos));
  *v = *ptr;
}

void VertexPack::ReadVector4(Vector4* v, const VertexPos& pos) const {
  CHECK(pos.slot != -1 && pos.index != -1);
  DCHECK(get_data_type(pos) == kVec4);
  Vector4* ptr = (Vector4*)(get_data_ptr(pos));
  *v = *ptr;
}

void VertexPack::ReadVector3Or4(Vector3* v, const VertexPos& pos) const {
  CHECK(pos.slot != -1 && pos.index != -1);
  if (get_data_type(pos) == kVec4) {
    Vector4* ptr = (Vector4*)(get_data_ptr(pos));
    v->x = ptr->x;
    v->y = ptr->y;
    v->z = ptr->z;
  } else if (get_data_type(pos) == kVec3) {
    Vector3* ptr = (Vector3*)(get_data_ptr(pos));
    *v = *ptr;
  } else {
    CHECK(false);
  }
}

VertexData* VertexPack::data() { 
  return vertex_data_.get();
}

const VertexDesc* VertexPack::desc() const{ 
  return vertex_data_->desc();
}

uint8* VertexPack::get_data_ptr(const VertexPos& pos) const {
  SlotVertexData* slot = vertex_data_->vertex_data_at(pos.slot);
  uint8* ptr = slot->vertex_data_at(index_);
  return ptr + slot->desc()->offset(pos.index);
}

DataFormat VertexPack::get_data_type(const VertexPos& pos) const {
  DCHECK(pos.slot != -1 && pos.index != -1);
  SlotVertexData* slot = vertex_data_->vertex_data_at(pos.slot);
  const VertexDesc::Desc* desc = slot->desc()->descs() + pos.index;
  return desc->type;
}

bool GetSemanticIndex(const std::string& name, int32 semantic_index, 
                      const VertexDesc* d, VertexPos* pos) {
  const VertexDesc::Desc* desc = d->descs();
  int32 index = 0;
  int32 prev_slot = 0;
  for (int32 i = 0; i < d->element_num(-1); ++i) {
    if (base::strncasecmp(name.c_str(), (desc + i)->name, name.length()) == 0
        && (desc + i)->semantic_index == semantic_index) {
      const VertexDesc::Desc* cur = (desc + i);
      if (prev_slot != desc->input_slot) {
        prev_slot = desc->input_slot;
        index = 0;
      }

      pos->slot = desc->input_slot;
      pos->index = index;
      return true;
    }
    index++;
  }
  pos->slot = -1;
  pos->index = -1;
  return false;
}
}  // namespace azer
