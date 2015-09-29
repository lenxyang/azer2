#pragma once

#include <vector>

#include "base/basictypes.h"
#include "azer/math/math.h"
#include "azer/render/render_system_enum.h"
#include "azer/render/vertex_buffer.h"

namespace azer {

class VertexPack {
 public:
  VertexPack(VertexData* data);
  VertexPack(int32 alignbytes, const std::vector<VertexDesc::Desc>& desc, 
             uint8* data, int32 data_size);
  VertexPack(int32 alignbytes, const VertexDesc::Desc* desc, int desc_count,
             uint8* data, int32 data_size);
  ~VertexPack();

  int32 size() const { return size_;}
  bool move(int32 offset);
  bool first();
  bool next(int32 step);
  bool end();
  uint8* data() { return data_;}
  const uint8* data() const { return data_;}
  uint8* current() { return current_;}
  const uint8* current() const { return current_;}
  int32 rest_bytes() const { return data_ + size_ - current_;}
  int32 index() const;

  void WriteFloat(float v, int32 column);
  void WriteVector2(const Vector2& v, int32 column);
  void WriteVector3(const Vector3& v, int32 column);
  void WriteVector4(const Vector4& v, int32 column);

  void ReadFloat(float* v, int32 column) const;
  void ReadVector2(Vector2* v, int32 column) const;
  void ReadVector3(Vector3* v, int32 column) const;
  void ReadVector4(Vector4* v, int32 column)const;

  int32 unit_size() const;
 private:
  void calc_offsets();
  int32 offset_of_column(int32 column) const;
  uint8* data_;
  uint8* current_;
  uint32 size_;
  uint32 unit_size_;
  std::vector<int32> offsets_;
  std::vector<VertexDesc::Desc> desc_;
  const int32 kAlignBytes;
  DISALLOW_COPY_AND_ASSIGN(VertexPack);
};

int32 memalign(int32 n, int32 align);
}  // namespace azer
