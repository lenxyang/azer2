#pragma once

#include <vector>
#include <memory>


#include "base/memory/ref_counted.h"
#include "azer/math/math.h"
#include "azer/base/export.h"
#include "azer/render/common.h"
#include "azer/render/gpu_buffer.h"

namespace azer {

class Renderer;

class GpuConstantsType {
 public:
  enum Type {
    kFloat = 1,
    kVector2 = kVec2,
    kVector3 = kVec3,
    kVector4 = kVec4,
    kInt,
    kIntVec2,
    kIntVec3,
    kIntVec4,
    kUint,
    kUintVec2,
    kUintVec3,
    kUintVec4,
    kBool,
    kBoolVec2,
    kBoolVec3,
    kBoolVec4,
    kMatrix2,
    kMatrix3,
    kMatrix4,
    kSampler1D,
    kSampler2D,
    kSampler3D,
    kSamplerCube,
    kSelfDefined,
  };
};

AZER_EXPORT int32_t GpuTableItemTypeSize(const GpuConstantsType::Type type);

class AZER_EXPORT GpuConstantsTable : public GpuBuffer {
 public:
  struct Desc {
    char name[64];
    GpuConstantsType::Type type;
    int element_size;
    int32_t num;
    uint32_t offset;

    Desc(const char* n, GpuConstantsType::Type t, int32_t off, int elenum)
        : type(t), element_size(-1),  num(elenum), offset(off) {
      strncpy(name, n, sizeof(name) - 1);
      element_size = GpuTableItemTypeSize(t);
    }

    Desc(const char* n, int32_t off, int size, int elenum)
        : type(GpuConstantsType::kSelfDefined)
        , element_size(size), num(elenum), offset(off) {
      strncpy(name, n, sizeof(name) - 1);
    }

    Desc(const Desc& desc) {
      strncpy(this->name, desc.name, sizeof(name));
      this->type = desc.type;
      this->element_size = desc.element_size;
      this->num = desc.num;
      this->offset = desc.offset;
    }
  };

  virtual ~GpuConstantsTable() {}
  virtual void flush(Renderer*) = 0;

  // set value to gpu constants
  void SetValue(int32_t idx, const void* value, int32_t size);
  void SetValueWithOffset(int32_t idx, int32_t offset, const void* value, int32_t size);
  void SetArrayItem(int32_t idx, int32_t arridx, const void* value, int32_t size);
  void SetArrayMultiItem(int32_t idx, int32_t arridx, const void* value, int32_t size);
  void SetMatrix(int32_t idx, const Matrix4* mtrl, int num);

  int32_t offset(int32_t index) const;
  int32_t size() const { return size_;}
 protected:
  GpuConstantsTable(int32_t num, const Desc* desc);
  GpuBufferLockDataPtr map(MapType flags) override;
  void unmap() override;

  struct Variable {
    Desc desc;
    int32_t size;
    int32_t element_size;
    int32_t offset;
    void* extra;
    Variable(const Desc& d, int32_t es, int32_t s, int32_t off)
        : desc(d), size(s), element_size(es), offset(off), extra(NULL) {}
  };

  /**
   * all try to modify data, should call this function
   */
  void SetData(int offset, const void* value, int32_t size);
  std::vector<Variable> constants_;
  std::unique_ptr<uint8_t[]> data_;
  int32_t size_;
  DISALLOW_COPY_AND_ASSIGN(GpuConstantsTable);
};

typedef scoped_refptr<GpuConstantsTable> GpuConstantsTablePtr;
AZER_EXPORT int32_t GpuTableItemDescSize(const GpuConstantsTable::Desc& desc);
}  // namespace
