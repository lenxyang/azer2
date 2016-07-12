#pragma once

#include <memory>
#include <string.h>
#include <vector>

#include "azer/base/export.h"
#include "azer/render/common.h"
#include "azer/render/gpu_buffer.h"

namespace azer {

class Renderer;
class RenderSystem;
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

class AZER_EXPORT VertexDesc : public ::base::RefCounted<VertexDesc> {
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
    int32_t input_slot;          // input slot
    int32_t instance_data_step;  // step
    bool aligned;              // aligned
  };

  explicit VertexDesc(const Desc* desc);
  VertexDesc(const Desc* desc, int desc_count);
  ~VertexDesc();

  // calc offset by name or index
  int32_t offset(const int32_t index) const;

  // size of single vertex 
  int32_t slot_count() const;
  int32_t stride(int32_t slot) const;
  int32_t element_slot(int32_t index) const;
  int32_t element_index_inslot(int32_t index) const;
  int32_t element_count_inslot(int32_t index) const;
  int32_t element_count() const;
  int32_t vertex_size() const;
  const Desc* descs() const;

  VertexDescPtr gen_slot_desc(int32_t index) const;

  // print Desc info
  friend std::ostream& operator << (std::ostream& os, const VertexDesc& data);

  // debug function
  std::string dump_vertex_data() const;
 private:
  void init(const Desc* desc, int desc_count);

  void ReCalc(int strip);
  std::unique_ptr<Desc[]> desc_;
  int32_t vertex_size_;
  int32_t slot_count_;

  typedef std::vector<int32_t> OffsetIndex;
  OffsetIndex offsets_idx_;
  std::vector<int32_t> slot_index_;
  std::vector<int32_t> slot_stride_;
  std::vector<int32_t> slot_element_;

  DISALLOW_COPY_AND_ASSIGN(VertexDesc);
};

class AZER_EXPORT SlotVertexData : public ::base::RefCounted<SlotVertexData> {
 public:
  SlotVertexData(VertexDesc* desc, int32_t vertex_count);
  void extend(int32_t count);

  uint8_t* next(const uint8_t* cur);
  const uint8_t* next(const uint8_t* cur) const;

  uint8_t* pointer();
  const uint8_t* pointer() const;

  uint8_t* vertex_data_at(int32_t index);
  const uint8_t* vertex_data_at(int32_t index) const;
  int32_t buffer_size() const;
  int32_t vertex_count() const;
  int32_t element_count() const;
  int32_t stride() const;

  const VertexDesc* vertex_desc() const;
  VertexDesc* vertex_desc();
 private:
  std::unique_ptr<uint8_t[]> data_;
  int32_t vertex_count_;
  VertexDescPtr desc_;
  DISALLOW_COPY_AND_ASSIGN(SlotVertexData);
};

class AZER_EXPORT VertexData : public ::base::RefCounted<VertexData> {
 public:
  VertexData(VertexDesc* desc);
  VertexData(VertexDesc* desc, int32_t vertex_count);
  ~VertexData();

  void extend(int32_t count);
  void set_slot_vertex_data(SlotVertexData* data, int32_t slot_index);
  SlotVertexData* vertex_data_at(int32_t index);

  int32_t vertex_count() const { return vertex_count_;}
  const VertexDesc* vertex_desc() const;
  VertexDesc* vertex_desc();
 private:
  void InitSlotFromDesc();
  VertexDescPtr desc_;
  int32_t vertex_count_;
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

class AZER_EXPORT VertexBuffer : public GpuBuffer {
 public:
  explicit VertexBuffer(const GpuResOptions &opt);
  virtual ~VertexBuffer();

  /**
   * 从 Lockable 继承的借口
   */
  virtual GpuResLockDataPtr map(MapType flags) = 0;
  virtual void unmap() = 0;

  int32_t vertex_count() const;
  int32_t buffer_size() const;
  int32_t element_size() const;
  VertexDesc* vertex_desc();
  const VertexDesc* vertex_desc() const;
  VertexLayout* vertex_layout();
 protected:
  VertexLayoutPtr layout_;
  int32_t element_size_;
  int32_t buffer_size_;
  int32_t vertex_count_;
  DISALLOW_COPY_AND_ASSIGN(VertexBuffer);
};

class AZER_EXPORT VertexBufferGroup : public ::base::RefCounted<VertexBufferGroup> {
 public:
  explicit VertexBufferGroup(VertexDesc* vdesc);
  ~VertexBufferGroup();

  bool validate() const;
  int32_t vertex_count() const { return vertex_count_;}
  int32_t vertex_buffer_count() const { return static_cast<int32_t>(vector_.size());}
  VertexBuffer* vertex_buffer_at(int32_t index);
  void add_vertex_buffer(VertexBuffer* vb);
  void add_vertex_buffer_at(VertexBuffer* vb, int32_t index);
  void remove_vertex_buffer_at(int32_t index);
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
  int32_t vertex_count_;
  DISALLOW_COPY_AND_ASSIGN(VertexBufferGroup);
};

AZER_EXPORT int32_t VertexTypeSize(DataFormat type);
AZER_EXPORT std::string DumpVertexDesc(const VertexDesc* desc);
AZER_EXPORT VertexBufferGroupPtr CreateVertexBufferGroup(
    const GpuResOptions& opt, VertexData* vdata);
}  // namespace azer
