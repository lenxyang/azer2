#pragma once

#include <string>
#include "base/basictypes.h"
#include "base/logging.h"

namespace wow {
class BytePacket;
class BytePackIterator {
 public:
  explicit BytePackIterator(const BytePacket& packet);
  BytePackIterator(const char* begin, int size);
  BytePackIterator(const BytePackIterator& iterator);

  BytePackIterator& operator = (const BytePackIterator& iter);
  int32 offset() const { return read_index_ - payload_;}
  void reset();
  enum {
    kSeekFromBeg,
    kSeekFromCur,
    kSeekFromEnd,
  };
  bool seek(int32 offset, int mode);
  bool end() const { return read_index_ == end_index_;}
  int32 rest_bytes() const { return end_index_ - read_index_;}
  const char* current() const { return read_index_;}
      
  bool ReadBool(bool* result) WARN_UNUSED_RESULT;
  bool ReadUint8(uint8* result) WARN_UNUSED_RESULT;
  bool ReadUint16(uint16* result) WARN_UNUSED_RESULT;
  bool ReadUint32(uint32* result) WARN_UNUSED_RESULT;
  bool ReadUint64(uint64* result) WARN_UNUSED_RESULT;

  bool ReadChar(char* result) WARN_UNUSED_RESULT;
  bool ReadInt16(int16* result) WARN_UNUSED_RESULT;
  bool ReadInt32(int32* result) WARN_UNUSED_RESULT;
  bool ReadInt64(int64* result) WARN_UNUSED_RESULT;

  bool ReadFloat(float* result) WARN_UNUSED_RESULT;
  bool ReadDouble(double* result) WARN_UNUSED_RESULT;
  bool ReadBytes(char* buf, int length) WARN_UNUSED_RESULT;
  bool ReadString(std::string* str) WARN_UNUSED_RESULT;
  bool SkipBytes(int32 size);

  BytePacket pack() const;
 private:
  const char* payload_;
  const char* read_index_;
  const char* end_index_;
};

class BytePacket {
 public:
  BytePacket(const char* data, int length);
  BytePacket(const BytePacket& pack);
  BytePacket& operator = (const BytePacket& pack);

  const char* payload() const { return payload_;}
  int payload_size() const { return payload_length_;}
 private:
  const char* payload_;
  int payload_length_;
};

class ScopedOffsetStore {
 public:
  ScopedOffsetStore(BytePackIterator* iter)
      : iter_(iter),
        prev_offset_(iter->offset()) {
  }
  
  ~ScopedOffsetStore() {
    iter_->seek(prev_offset_, BytePackIterator::kSeekFromBeg);
  }
  private:
  BytePackIterator* iter_;
  int32 prev_offset_;
};

class ScopedOffsetCheck {
 public:
  ScopedOffsetCheck(BytePackIterator* iter, int32 expect) 
      : iter_(iter),
        expect_(expect) {
    prev_offset_ = iter->offset();
  }

  ~ScopedOffsetCheck() {
    int32 offset = iter_->offset() - prev_offset_;
    CHECK(expect_ == offset);
  }
 private:
  BytePackIterator* iter_;
  int32 prev_offset_;
  int32 expect_;
  DISALLOW_COPY_AND_ASSIGN(ScopedOffsetCheck);
};
}  // namespace wow
