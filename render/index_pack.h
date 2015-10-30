#pragma once

#include "azer/render/indices_buffer.h"

namespace azer {
class AZER_EXPORT IndexPack {
 public:
  IndexPack(IndicesData* data);
  IndexPack(uint8* data, int32 data_size, IndicesData::IndexType type);

  bool WriteAndAdvance(int32 value);
  bool ReadAndAdvance(uint32 *value) const;
  uint32 ReadAndAdvanceOrDie() const;

  void write(int32 value);
  bool advance() const;
  uint32 value() const;
  IndicesData::IndexType type() const { return type_;}

  uint8* current() { return current_;}
  const uint8* current() const { return current_;}
  int32 count() const;
 private:
  uint8* data_;
  mutable uint8* current_;
  int32 data_size_;
  IndicesData::IndexType type_;
  DISALLOW_COPY_AND_ASSIGN(IndexPack);
};
}  // namespace azer
