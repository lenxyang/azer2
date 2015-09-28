#include "azer/util/byte_pack.h"


namespace wow {

BytePackIterator::BytePackIterator(const BytePacket& packet)
    : payload_(packet.payload()),
      read_index_(packet.payload()),
      end_index_(packet.payload() + packet.payload_size()) {
}

BytePackIterator::BytePackIterator(const char* begin, int size)
    : payload_(begin),
      read_index_(begin),
      end_index_(begin + size) {
}

BytePackIterator::BytePackIterator(const BytePackIterator& iterator) {
  *this = iterator;
}

BytePackIterator& BytePackIterator::operator = (const BytePackIterator& iter) {
  this->payload_ = iter.payload_;
  this->read_index_ = iter.read_index_;
  this->end_index_ = iter.end_index_;
  return *this;
}

bool BytePackIterator::ReadBool(bool* result) {
  return ReadBytes((char*)result, sizeof(*result));
}

bool BytePackIterator::ReadUint8(uint8* result) {
  return ReadBytes((char*)result, sizeof(*result));
}

bool BytePackIterator::ReadUint16(uint16* result) {
  return ReadBytes((char*)result, sizeof(*result));
}

bool BytePackIterator::ReadUint32(uint32* result) {
  return ReadBytes((char*)result, sizeof(*result));
}

bool BytePackIterator::ReadUint64(uint64* result) {
  return ReadBytes((char*)result, sizeof(*result));
}


bool BytePackIterator::ReadChar(char* result) {
  if (read_index_ < end_index_) {
    *result = *read_index_;
    read_index_++;
    return true;
  } else {
    return false;
  }
}

bool BytePackIterator::ReadInt16(int16* result) {
  return ReadBytes((char*)result, sizeof(*result));
}

bool BytePackIterator::ReadInt32(int32* result) {
  return ReadBytes((char*)result, sizeof(*result));
}

bool BytePackIterator::ReadInt64(int64* result) {
  return ReadBytes((char*)result, sizeof(*result));
}

bool BytePackIterator::ReadFloat(float* result) {
  return ReadBytes((char*)result, sizeof(*result));
}

bool BytePackIterator::ReadDouble(double* result) {
  return ReadBytes((char*)result, sizeof(*result));
}

bool BytePackIterator::SkipBytes(int32 size) {
  if (read_index_ + size <= end_index_) {
    read_index_ += size;
    return true;
  } else {
    return false;
  }
}

bool BytePackIterator::ReadBytes(char* buf, int size) {
  char* cur = buf;
  if (read_index_ + size <= end_index_) {
    memcpy(buf, read_index_, size);
    read_index_ += size;
    return true;
  } else {
    return false;
  }
}

bool BytePackIterator::ReadString(std::string* str) {
  char c;
  do {
    if (!ReadChar(&c))
      return false;
    str->push_back(c);
  } while (c != '\0');
  return true;
}

BytePacket BytePackIterator::pack() const {
  BytePacket pack(payload_, end_index_ - payload_);
  return pack;
}

void BytePackIterator::reset() {
  read_index_ = payload_;
}

bool BytePackIterator::seek(int32 offset, int mode) {
  switch (mode) {
    case kSeekFromBeg:
      read_index_ = payload_ + offset;
      break;
    case kSeekFromCur:
      read_index_ += offset;
      break;
    case kSeekFromEnd:
      read_index_ -= offset;
      break;
    default:
      NOTREACHED();
      break;
  }

  return read_index_ <= end_index_ && read_index_ >= payload_;
}

// class BytePacket
BytePacket::BytePacket(const char* data, int length)
    : payload_(data),
      payload_length_(length) {
}

BytePacket::BytePacket(const BytePacket& pack) {
  *this =  pack;
}

BytePacket& BytePacket::operator = (const BytePacket& pack) {
  this->payload_ = pack.payload_;
  this->payload_length_ = pack.payload_length_;
  return *this;
}

}
