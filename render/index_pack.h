#pragma once

#include "azer/render/indices_buffer.h"

namespace azer {
class AZER_EXPORT IndexPack {
 public:
  IndexPack(IndicesData* data);

  bool WriteAndAdvance(int32_t value);
  bool ReadAndAdvance(uint32_t *value) const;
  uint32_t ReadAndAdvanceOrDie() const;

  void write(int32_t value);
  void move(int32_t step);
  bool advance(int32_t step = 1) const;
  uint32_t value() const;
  uint32_t value(int32_t index) const;

  IndexType type() const { return idata_->type();}
  int32_t index() const;
  uint8* pointer() { return idata_->pointer();}
  const uint8* pointer() const { return idata_->pointer();}
  uint8* current() { return idata_->pointer() + offset_;}
  const uint8* current() const { return idata_->pointer() + offset_;}
  int32_t count() const { return idata_->count();}
  int32_t step_size() const { return idata_->unit_size();}
  IndicesData* data() { return idata_;}
 private:
  IndicesData* idata_;
  mutable uint64 offset_;
  DISALLOW_COPY_AND_ASSIGN(IndexPack);
};
}  // namespace azer
