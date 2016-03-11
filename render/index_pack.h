#pragma once

#include "azer/render/indices_buffer.h"

namespace azer {
class AZER_EXPORT IndexPack {
 public:
  IndexPack(IndicesData* data);
  IndexPack(uint8* data, int32 data_size, IndexType type);

  bool WriteAndAdvance(int32 value);
  bool ReadAndAdvance(uint32 *value) const;
  uint32 ReadAndAdvanceOrDie() const;

  void write(int32 value);
  void move(int32 step);
  bool advance(int32 step = 1) const;
  uint32 value() const;
  uint32 value(int32 index) const;
  IndexType type() const { return type_;}

  uint8* current() { return current_;}
  const uint8* current() const { return current_;}
  int32 count() const;
 private:
  int32 step_size() const;
  uint8* data_;
  mutable uint8* current_;
  int32 data_size_;
  IndexType type_;
  DISALLOW_COPY_AND_ASSIGN(IndexPack);
};
}  // namespace azer
