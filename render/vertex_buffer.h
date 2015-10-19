#pragma once

#include <memory>
#include <string.h>
#include <vector>

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/base/resource.h"
#include "azer/render/render_system_enum.h"
#include "azer/render/hardware_buffer.h"

namespace azer {

class Renderer;
class SlotVertexData;
class VertexDesc;
typedef scoped_refptr<SlotVertexData> SlotVertexDataPtr;
typedef scoped_refptr<VertexDesc> VertexDescPtr;

class AZER_EXPORT VertexDesc : public Resource {
 public:
  /**
   * 对定点结构的定义
   * 参考 D3D11_INPUT_ELEMENT_DESC(Direct3D 11)
   * 参考 Opengl glAttributePointer()
   */
  struct AZER_EXPORT Desc {
    char name[64];             // semantic_name
    int semantic_index;        // semantic_index
    DataFormat type;           // type
    int32 input_slot;         // input slot
    int32 instance_data_step;  // step
    bool aligned;              // aligned
  };

  VertexDesc(const Desc* desc, int desc_num);
  ~VertexDesc();

  // calc offset by name or index
  int32 offset(const int32 index) const;

  // size of single vertex 
  int32 slot_count() const;
  int32 stride(int32 slot) const;
  int32 element_num(int32 index) const;
  int32 vertex_size() const;
  const Desc* descs() const;

  VertexDescPtr gen_slot_desc(int32 index) const;

  // print Desc info
  friend std::ostream& operator << (std::ostream& os, const VertexDesc& data);

  // debug function
  std::string dump_vertex_data() const;
 private:
  void init(const Desc* desc, int desc_num);

  /**
   * 此函数用于根据 strip 跟新 VertexDesc
   * 当用户调用 Map 时，GPU 的数据与 CPU 的数据，内存很有可能是
   * 不匹配的，此函数将根据 GPU 返回的长度重新计算 Desc
   */
  void ReCalc(int strip);
  scoped_ptr<Desc[]> desc_;
  int32 vertex_size_;
  int32 slot_count_;

  typedef std::vector<int32> OffsetIndex;
  OffsetIndex offsets_idx_;
  std::vector<int32> slot_index_;
  std::vector<int32> slot_stride_;
  std::vector<int32> slot_element_;

  DISALLOW_COPY_AND_ASSIGN(VertexDesc);
};

class AZER_EXPORT SlotVertexData : public Resource {
 public:
  SlotVertexData(const VertexDescPtr& desc, int32 vertex_num);

  uint8* next(const uint8* cur);
  const uint8* next(const uint8* cur) const;

  uint8* pointer();
  const uint8* pointer() const;
  int32 buffer_size() const;
  int32 vertex_num() const;
  int32 element_num() const;
  int32 stride() const;

  const VertexDesc* desc() const;
  VertexDesc* desc();
 private:
  std::unique_ptr<uint8[]> data_;
  int32 vertex_num_;
  VertexDescPtr desc_;
  DISALLOW_COPY_AND_ASSIGN(SlotVertexData);
};

class AZER_EXPORT VertexData : public ::base::RefCounted<VertexData> {
 public:
  VertexData(const VertexDescPtr& desc, int32 vertex_num);
  ~VertexData();

  void InitSlotFromDesc();
  void set_slot_vertex_data(SlotVertexDataPtr data, int32 slot_index);
  SlotVertexData* vertex_data_at(int32 index);

  const VertexDesc* desc() const;
 private:
  VertexDescPtr desc_;
  int32 vertex_num_;
  std::vector<SlotVertexDataPtr> vector_;
  DISALLOW_COPY_AND_ASSIGN(VertexData);
};

class VertexBuffer;
typedef scoped_refptr<VertexBuffer> VertexBufferPtr;

class AZER_EXPORT VertexBuffer : public HardwareBuffer {
 public:
  struct AZER_EXPORT Options {
    char name[128];
    GraphicBuffer::Usage usage;
    CPUAccess cpu_access;  // defined render_system
    Options();
  };

  explicit VertexBuffer(const Options &opt);
  virtual ~VertexBuffer();

  /**
   * 从 Lockable 继承的借口
   */
  virtual HardwareBufferDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;

  int32 vertex_num() const { return vertex_num_;}
  int32 buffer_size() const { return vertex_num_;}
  int32 element_size() const { return element_size_;}
 protected:
  const Options options_;
  int32 element_size_;
  int32 buffer_size_;
  int32 vertex_num_;
  DISALLOW_COPY_AND_ASSIGN(VertexBuffer);
};

int32 AZER_EXPORT VertexTypeSize(DataFormat type);

class AZER_EXPORT VertexBufferGroup : public ::base::RefCounted<VertexBufferGroup> {
 public:
  VertexBufferGroup(VertexDescPtr vdesc);
  ~VertexBufferGroup();

  int32 vertex_buffer_count() const;
  VertexBuffer* vertex_buffer_at(int32 index);
  void add_vertex_buffer(VertexBufferPtr vb);
  void add_vertex_buffer_at(VertexBufferPtr vb, int32 index);
  void remove_vertex_buffer_at(int32 index);
 private:
  VertexDescPtr vdesc_;
  std::vector<VertexBufferPtr> vector_;
  DISALLOW_COPY_AND_ASSIGN(VertexBufferGroup);
};

typedef scoped_refptr<VertexBufferGroup> VertexBufferGroupPtr;
}  // namespace azer
