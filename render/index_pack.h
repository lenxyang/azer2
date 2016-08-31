#pragma once

#include "azer/render/indices_buffer.h"

namespace azer {
class EntityData;

class AZER_EXPORT IndexPack {
 public:
  explicit IndexPack(IndicesData* data);
  explicit IndexPack(EntityData* data);

  bool WriteAndAdvance(int value);
  bool ReadAndAdvance(uint32_t *value) const;
  uint32_t ReadAndAdvanceOrDie() const;

  void write(int value);
  void move(int step);
  bool advance(int step = 1) const;
  uint32_t value() const;
  uint32_t value(int index) const;

  IndexType type() const { return idata_->type();}
  int32_t index() const;
  uint8_t* pointer() { return idata_->pointer();}
  const uint8_t* pointer() const { return idata_->pointer();}
  uint8_t* current() { return idata_->pointer() + offset_;}
  const uint8_t* current() const { return idata_->pointer() + offset_;}
  int32_t count() const { return idata_->count();}
  int32_t step_size() const { return idata_->unit_size();}
  IndicesData* data() { return idata_;}
 private:
  IndicesData* idata_;
  mutable uint64_t offset_;
  DISALLOW_COPY_AND_ASSIGN(IndexPack);
};
}  // namespace azer
