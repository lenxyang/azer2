#pragma once

#include "azer/render/indices_buffer.h"

namespace azer {
class AZER_EXPORT IndexPack {
 public:
  IndexPack(IndicesData* data);

  bool WriteAndAdvance(int32 value);
  bool ReadAndAdvance(uint32 *value) const;
  uint32 ReadAndAdvanceOrDie() const;

  void write(int32 value);
  void move(int32 step);
  bool advance(int32 step = 1) const;
  uint32 value() const;
  uint32 value(int32 index) const;

  IndexType type() const { return idata_->type();}
  int32 index() const;
  uint8* pointer() { return idata_->pointer();}
  const uint8* pointer() const { return idata_->pointer();}
  uint8* current() { return idata_->pointer() + offset_;}
  const uint8* current() const { return idata_->pointer() + offset_;}
  int32 count() const { return idata_->count();}
  int32 step_size() const { return idata_->unit_size();}
  IndicesData* data() { return idata_;}
 private:
  IndicesData* idata_;
  mutable uint64 offset_;
  DISALLOW_COPY_AND_ASSIGN(IndexPack);
};
}  // namespace azer
