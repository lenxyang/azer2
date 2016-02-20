#pragma once

#include <memory>
#include <string.h>
#include <vector>

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/base/resource.h"
#include "azer/render/common.h"
#include "azer/render/hardware_buffer.h"

namespace azer {

class Renderer;
class SlotVertexData;
class VertexData;
class VertexDesc;
class VertexBuffer;
class VertexBufferGroup;
class VertexLayout;
typedef scoped_refptr<SlotVertexData> SlotVertexDataPtr;
typedef scoped_refptr<VertexData> VertexDataPtr;
typedef scoped_refptr<VertexDesc> VertexDescPtr;
typedef scoped_refptr<VertexBuffer> VertexBufferPtr;
typedef scoped_refptr<VertexBufferGroup> VertexBufferGroupPtr;
typedef scoped_refptr<VertexLayout> VertexLayoutPtr;

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
    int32 input_slot;          // input slot
    int32 instance_data_step;  // step
    bool aligned;              // aligned
  };

  explicit VertexDesc(const Desc* desc);
  VertexDesc(const Desc* desc, int desc_count);
  ~VertexDesc();

  // calc offset by name or index
  int32 offset(const int32 index) const;

  // size of single vertex 
  int32 slot_count() const;
  int32 stride(int32 slot) const;
  int32 element_slot(int32 index) const;
  int32 element_index_inslot(int32 index) const;
  int32 element_count_inslot(int32 index) const;
  int32 element_count() const;
  int32 vertex_size() const;
  const Desc* descs() const;

  VertexDescPtr gen_slot_desc(int32 index) const;

  // print Desc info
  friend std::ostream& operator << (std::ostream& os, const VertexDesc& data);

  // debug function
  std::string dump_vertex_data() const;
 private:
  void init(const Desc* desc, int desc_count);

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
  SlotVertexData(VertexDesc* desc, int32 vertex_count);

  uint8* next(const uint8* cur);
  const uint8* next(const uint8* cur) const;

  uint8* pointer();
  const uint8* pointer() const;

  uint8* vertex_data_at(int32 index);
  const uint8* vertex_data_at(int32 index) const;
  int32 buffer_size() const;
  int32 vertex_count() const;
  int32 element_count() const;
  int32 stride() const;

  const VertexDesc* vertex_desc() const;
  VertexDesc* vertex_desc();
 private:
  std::unique_ptr<uint8[]> data_;
  int32 vertex_count_;
  VertexDescPtr desc_;
  DISALLOW_COPY_AND_ASSIGN(SlotVertexData);
};

class AZER_EXPORT VertexData : public ::base::RefCounted<VertexData> {
 public:
  VertexData(VertexDesc* desc);
  VertexData(VertexDesc* desc, int32 vertex_count);
  ~VertexData();

  void set_slot_vertex_data(SlotVertexData* data, int32 slot_index);
  SlotVertexData* vertex_data_at(int32 index);

  int32 vertex_count() const { return vertex_count_;}
  const VertexDesc* vertex_desc() const;
  VertexDesc* vertex_desc();
 private:
  void InitSlotFromDesc();
  VertexDescPtr desc_;
  int32 vertex_count_;
  std::vector<SlotVertexDataPtr> vector_;
  DISALLOW_COPY_AND_ASSIGN(VertexData);
};

class AZER_EXPORT VertexLayout : public ::base::RefCounted<VertexLayout> {
 public:
  VertexLayout(VertexDesc* desc) : desc_(desc) {}
  virtual ~VertexLayout() {}
  // just for cannot be init
  virtual bool Init(RenderSystem* rs) = 0;
  VertexDesc* vertex_desc() { return desc_.get();}
  const VertexDesc* vertex_desc() const { return desc_.get();}
 protected:
  VertexDescPtr desc_;
  DISALLOW_COPY_AND_ASSIGN(VertexLayout);
};

class AZER_EXPORT VertexBuffer : public HardwareBuffer {
 public:
  explicit VertexBuffer(const HBufferOptions &opt);
  virtual ~VertexBuffer();

  /**
   * 从 Lockable 继承的借口
   */
  virtual HardwareBufferDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;

  int32 vertex_count() const;
  int32 buffer_size() const;
  int32 element_size() const;
  VertexDesc* vertex_desc();
  const VertexDesc* vertex_desc() const;
  VertexLayout* vertex_layout();
 protected:
  VertexLayoutPtr layout_;
  int32 element_size_;
  int32 buffer_size_;
  int32 vertex_count_;
  DISALLOW_COPY_AND_ASSIGN(VertexBuffer);
};

class AZER_EXPORT VertexBufferGroup : public ::base::RefCounted<VertexBufferGroup> {
 public:
  explicit VertexBufferGroup(VertexDesc* vdesc);
  ~VertexBufferGroup();

  bool validate() const;
  int32 vertex_count() const { return vertex_count_;}
  int32 vertex_buffer_count() const { return static_cast<int32>(vector_.size());}
  VertexBuffer* vertex_buffer_at(int32 index);
  void add_vertex_buffer(VertexBuffer* vb);
  void add_vertex_buffer_at(VertexBuffer* vb, int32 index);
  void remove_vertex_buffer_at(int32 index);
  std::vector<VertexBufferPtr>* get_vb_vector();

  VertexDesc* vertex_desc();
  const VertexDesc* vertex_desc() const;
  VertexLayout* vertex_layout();
  const VertexLayout* vertex_layout() const;
 protected:
  virtual void OnVertexBufferChanged() = 0;
  VertexLayoutPtr layout_;
 private:
  VertexDescPtr vdesc_;
  std::vector<VertexBufferPtr> vector_;
  int32 vertex_count_;
  DISALLOW_COPY_AND_ASSIGN(VertexBufferGroup);
};

AZER_EXPORT int32 VertexTypeSize(DataFormat type);
AZER_EXPORT std::string DumpVertexDesc(const VertexDesc* desc);
AZER_EXPORT VertexBufferGroupPtr CreateVertexBufferGroup(
    const HBufferOptions& opt, VertexData* vdata);
}  // namespace azer
