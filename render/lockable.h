#pragma once

#include <memory>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/render_system_enum.h"

namespace azer {

class Renderer;

class AZER_EXPORT LockData : public ::base::RefCounted<LockData> {
 public:
  LockData() : data_(0), row_size_(-1), size_(-1) {
  }

  uint8* data_ptr() const { return data_;}
  int32_t row_size() const { return row_size_;}
  int32_t column_num() const { return column_num_;}
 private:
  uint8* data_;
  int32_t row_size_;
  int32_t column_num_;
  int32_t size_;

  friend class Lockable;
  DISALLOW_COPY_AND_ASSIGN(LockData);
};

typedef scoped_refptr<LockData> LockDataPtr;

class AZER_EXPORT HardwareBuffer : public ::base::RefCounted<HardwareBuffer> {
 public:
  HardwareBuffer() {}
  virtual ~HardwareBuffer() {}

  virtual LockDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;
 protected:
  void SetLockDataPtr(void* ptr, LockData* data) {
    data->data_ = (uint8*)ptr;
  }
  void SetLockDataRowSize(int32_t size, LockData* data) {
    data->row_size_ = size;
  }
  void SetLockDataColumnNum(int32_t num, LockData* data) {
    data->column_num_ = num;
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(Lockable);
};

class AZER_EXPORT AutoBufferLock {
 public:
  AutoBufferLock(Lockable* lockable, MapType flags, Renderer* renderer)
      : lockable_(lockable) {
    data_ = lockable_->map(flags);
  }
  ~AutoBufferLock() {
    if (data_.get()) {
      lockable_->unmap();
    }
  }
 private:
  LockDataPtr data_;
  Lockable* lockable_;
  DISALLOW_COPY_AND_ASSIGN(AutoBufferLock);
};
}  // namespace azer
