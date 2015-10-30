#pragma once

#include <vector>

#include "base/basictypes.h"
#include "azer/math/math.h"
#include "azer/render/render_system_enum.h"
#include "azer/render/vertex_buffer.h"

namespace azer {

struct AZER_EXPORT VertexPos {
  int32 slot;
  int32 index;

  VertexPos();
  VertexPos(int32 slot, int32 index);
};
bool AZER_EXPORT GetSemanticIndex(const std::string& name, int32 semantic_index, 
                                  const VertexDesc* desc, VertexPos* pos);

class AZER_EXPORT VertexPack {
 public:
  VertexPack(VertexData* data);
  VertexPack(SlotVertexData* data);
  ~VertexPack();

  bool move(int32 offset);
  bool first();
  bool next(int32 step);
  bool end();
  int32 index() const;

  void WriteFloat(float v, const VertexPos& pos);
  void WriteVector2(const Vector2& v, const VertexPos& pos);
  void WriteVector3(const Vector3& v, const VertexPos& pos);
  void WriteVector4(const Vector4& v, const VertexPos& pos);

  void ReadFloat(float* v, const VertexPos& pos) const;
  void ReadVector2(Vector2* v, const VertexPos& pos) const;
  void ReadVector3(Vector3* v, const VertexPos& pos) const;
  void ReadVector4(Vector4* v, const VertexPos& pos) const;

  VertexData* data();
  const VertexDesc* desc() const;
 private:
  void calc_offsets();
  uint8* get_data_ptr(const VertexPos& pos) const;
  DataFormat get_data_type(const VertexPos& pos) const;
  int32 index_;
  VertexDataPtr vertex_data_;
  const int32 kAlignBytes;
  DISALLOW_COPY_AND_ASSIGN(VertexPack);
};

}  // namespace azer
