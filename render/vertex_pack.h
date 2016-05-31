#pragma once

#include <vector>


#include "azer/math/math.h"
#include "azer/render/common.h"
#include "azer/render/vertex_buffer.h"

namespace azer {

struct AZER_EXPORT VertexPos {
  int32_t slot;
  int32_t index;

  VertexPos();
  VertexPos(int32_t slot, int32_t index);
};
bool AZER_EXPORT GetSemanticIndex(const std::string& name, int32_t semantic_index, 
                                  const VertexDesc* desc, VertexPos* pos);

class AZER_EXPORT VertexPack {
 public:
  VertexPack(VertexData* data);
  VertexPack(SlotVertexData* data);
  ~VertexPack();

  bool move(int32_t offset);
  bool first();
  bool next(int32_t step);
  bool end();
  int32_t index() const;

  void Writeint(int32_t v, const VertexPos& pos);
  void WriteIntVec2(const IntVec2& v, const VertexPos& pos);
  void WriteIntVec3(const IntVec3& v, const VertexPos& pos);
  void WriteIntVec4(const IntVec4& v, const VertexPos& pos);
  void WriteFloat(float v, const VertexPos& pos);
  void WriteVector2(const Vector2& v, const VertexPos& pos);
  void WriteVector3(const Vector3& v, const VertexPos& pos);
  void WriteVector4(const Vector4& v, const VertexPos& pos);
  void WriteVector3Or4(const Vector4& v, const VertexPos& pos) const;

  void ReadInt(int32_t* v, const VertexPos& pos) const;
  void ReadIntVec2(IntVec2* v, const VertexPos& pos) const;
  void ReadIntVec3(IntVec3* v, const VertexPos& pos) const;
  void ReadIntVec4(IntVec4* v, const VertexPos& pos) const;
  void ReadFloat(float* v, const VertexPos& pos) const;
  void ReadVector2(Vector2* v, const VertexPos& pos) const;
  void ReadVector3(Vector3* v, const VertexPos& pos) const;
  void ReadVector4(Vector4* v, const VertexPos& pos) const;
  void ReadVector3Or4(Vector3* v, const VertexPos& pos) const;

  VertexData* data();
  const VertexDesc* desc() const;
 private:
  void calc_offsets();
  uint8_t* get_data_ptr(const VertexPos& pos) const;
  DataFormat get_data_type(const VertexPos& pos) const;
  int32_t index_;
  VertexDataPtr vertex_data_;
  const int32_t kAlignBytes;
  DISALLOW_COPY_AND_ASSIGN(VertexPack);
};

}  // namespace azer
