#include "azer/render/vertex_pack.h"

#include "base/logging.h"
#include "base/strings/string_util.h"

namespace azer {
int32_t memalign(int32_t n, int32_t align) {
  return (n + align - 1) & (~(align - 1));
}

VertexPos::VertexPos() : slot(-1), index(-1) {}
VertexPos::VertexPos(int32_t s, int32_t i) : slot(s), index(i) {}

VertexPack::VertexPack(VertexData* data) 
    : vertex_data_(data),
      index_(-1),
      kAlignBytes(1) {
}

VertexPack::VertexPack(SlotVertexData* data)
    : index_(-1),
      kAlignBytes(1) {
  vertex_data_ = new VertexData(data->vertex_desc(), data->vertex_count());
  vertex_data_->set_slot_vertex_data(SlotVertexDataPtr(data), 0);
}

VertexPack::~VertexPack() {
}

int32_t VertexPack::index() const {
  return index_;
}

bool VertexPack::first() {
  index_ = 0;
  return index_ < vertex_data_->vertex_count();
}

bool VertexPack::move(int32_t offset) {
  first();
  index_ = offset;
  return index_ < vertex_data_->vertex_count();
}

bool VertexPack::next(int32_t step) {
  index_ += step;
  return index_ < vertex_data_->vertex_count();
}

bool VertexPack::end() {
  return !(index_ < vertex_data_->vertex_count());
}

void VertexPack::Writeint(int32_t v, const VertexPos& pos) {
  if (pos.slot == -1 || pos.index == -1)
    return;
  DCHECK(get_data_type(pos) == kInt);
  uint8* ptr = get_data_ptr(pos);
  *((int32*)ptr) = v;
}

void VertexPack::WriteIntVec2(const IntVec2& v, const VertexPos& pos) {
  if (pos.slot == -1 || pos.index == -1)
    return;

  uint8* ptr = get_data_ptr(pos);
  DCHECK(get_data_type(pos) == kIntVec2);
  IntVec2* vec = (IntVec2*)ptr;
  *vec = v;
}

void VertexPack::WriteIntVec3(const IntVec3& v, const VertexPos& pos) {
  if (pos.slot == -1 || pos.index == -1)
    return;

  DCHECK(get_data_type(pos) == kIntVec3);
  uint8* ptr = get_data_ptr(pos);
  IntVec3* vec = (IntVec3*)ptr;
  *vec = v;
}

void VertexPack::WriteIntVec4(const IntVec4& v, const VertexPos& pos) {
  if (pos.slot == -1 || pos.index == -1)
    return;

  if (get_data_type(pos) == kIntVec4) {
    uint8* ptr = get_data_ptr(pos);
    IntVec4* vec = (IntVec4*)ptr;
    *vec = v;
  } else if (get_data_type(pos) == kByteInt) {
    uint32* ptr = (uint32*)get_data_ptr(pos);
    int32_t nv = (v.x & 0x000000FF);
    nv |= (v.y & 0x0000FF00) >> 8;
    nv |= (v.z & 0x00FF0000) >> 16;
    nv |= (v.w & 0xFF000000) >> 24;
    *ptr = nv;
  } else {
    CHECK(false);
  }
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

  if (get_data_type(pos) == kVec4) {
    uint8* ptr = get_data_ptr(pos);
    Vector4* vec = (Vector4*)ptr;

    *vec = v;
  } else if (get_data_type(pos) == kByteVec4) {
    uint32* ptr = (uint32*)get_data_ptr(pos);
    uint32_t x = ((uint32)(v.x * 255));
    uint32_t y = ((uint32)(v.y * 255)) >> 8;
    uint32_t z = ((uint32)(v.z * 255)) >> 16;
    uint32_t w = ((uint32)(v.w * 255)) >> 24;
    *ptr = x | y | z | w;
  } else {
    CHECK(false);
  }
}

void VertexPack::WriteVector3Or4(const Vector4& v, const VertexPos& pos) const {
  if (pos.slot == -1 || pos.index == -1)
    return;

  if (get_data_type(pos) == kVec4) {
    uint8* ptr = get_data_ptr(pos);
    Vector4* vec = (Vector4*)ptr;
    *vec = v;
  } else if (get_data_type(pos) == kVec3) {
    uint8* ptr = get_data_ptr(pos);
    Vector3* vec = (Vector3*)ptr;
    vec->x = v.x;
    vec->y = v.y;
    vec->z = v.z;
  } else {
    CHECK(false);
  }
}

void VertexPack::ReadInt(int32* v, const VertexPos& pos) const {
  CHECK(pos.slot != -1 && pos.index != -1);
  DCHECK(get_data_type(pos) == kInt);
  int32* ptr = (int32*)(get_data_ptr(pos));
  *v = *ptr;
}

void VertexPack::ReadIntVec2(IntVec2* v, const VertexPos& pos) const {
  CHECK(pos.slot != -1 && pos.index != -1);
  DCHECK(get_data_type(pos) == kIntVec2);
  IntVec2* ptr = (IntVec2*)(get_data_ptr(pos));
  *v = *ptr;
}

void VertexPack::ReadIntVec3(IntVec3* v, const VertexPos& pos) const {
  CHECK(pos.slot != -1 && pos.index != -1);
  DCHECK(get_data_type(pos) == kIntVec3);
  IntVec3* ptr = (IntVec3*)(get_data_ptr(pos));
  *v = *ptr;
}

void VertexPack::ReadIntVec4(IntVec4* v, const VertexPos& pos) const {
  CHECK(pos.slot != -1 && pos.index != -1);
  if (get_data_type(pos) == kIntVec4) {
    IntVec4* ptr = (IntVec4*)(get_data_ptr(pos));
    *v = *ptr;
  } else if (get_data_type(pos) == kByteIntVec4) {
    int32_t i32 = *(int32*)(get_data_ptr(pos));
    v->x = (0x000000ff & i32);
    v->y = (0x0000ff00 & i32) >> 8;
    v->z = (0x00ff0000 & i32) >> 16;
    v->w = (0xff000000 & i32) >> 24;
  } else {
    NOTREACHED();
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
  if (get_data_type(pos) == kVec4) {
    Vector4* ptr = (Vector4*)(get_data_ptr(pos));
    *v = *ptr;
  } else if (get_data_type(pos) == kByteVec4) {
    int32_t i32 = *(int32*)(get_data_ptr(pos));
    float x = (0x000000ff & i32);
    float y = (0x0000ff00 & i32) >> 8;
    float z = (0x00ff0000 & i32) >> 16;
    float w = (0xff000000 & i32) >> 24;
    *v = Vector4(x / 255.0f, y / 255.0f, z / 255.0f, w / 255.0f);
  } else {
    NOTREACHED();
  }
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
  return vertex_data_->vertex_desc();
}

uint8* VertexPack::get_data_ptr(const VertexPos& pos) const {
  SlotVertexData* slot = vertex_data_->vertex_data_at(pos.slot);
  uint8* ptr = slot->vertex_data_at(index_);
  return ptr + slot->vertex_desc()->offset(pos.index);
}

DataFormat VertexPack::get_data_type(const VertexPos& pos) const {
  DCHECK(pos.slot != -1 && pos.index != -1);
  SlotVertexData* slot = vertex_data_->vertex_data_at(pos.slot);
  const VertexDesc::Desc* desc = slot->vertex_desc()->descs() + pos.index;
  return desc->type;
}

bool GetSemanticIndex(const std::string& name, int32_t semantic_index, 
                      const VertexDesc* d, VertexPos* pos) {
  const VertexDesc::Desc* desc = d->descs();
  int32_t index = 0;
  int32_t prev_slot = 0;
  for (int32_t i = 0; i < d->element_count(); ++i) {
    if (base::strncasecmp(name.c_str(), (desc + i)->name, name.length()) == 0
        && (desc + i)->semantic_index == semantic_index) {
      const VertexDesc::Desc* cur = (desc + i);
      if (prev_slot != desc->input_slot) {
        prev_slot = desc->input_slot;
        index = 0;
      }

      pos->slot = d->element_slot(index);
      pos->index = d->element_index_inslot(index);
      return true;
    }
    index++;
  }
  pos->slot = -1;
  pos->index = -1;
  return false;
}
}  // namespace azer
