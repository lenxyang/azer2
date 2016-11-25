#pragma once

#include <memory>
#include <string.h>
#include <vector>

#include "azer/base/export.h"
#include "azer/render/common.h"
#include "azer/render/gpu_resource.h"

namespace azer {
class RenderSystem;
class VertexDesc;
class VertexLayout;
typedef scoped_refptr<VertexDesc> VertexDescPtr;
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
    int input_slot;          // input slot
    int instance_data_step;  // step
    bool aligned;              // aligned
  };

  explicit VertexDesc(const Desc* desc);
  VertexDesc(const Desc* desc, int desc_count);
  ~VertexDesc();

  // calc offset by name or index
  int offset(const int index) const;

  // size of single vertex 
  int slot_count() const;
  int stride(int slot) const;
  int element_slot(int index) const;
  int element_index_inslot(int index) const;
  int element_count_inslot(int index) const;
  int element_count() const;
  int vertex_size() const;
  const Desc* descs() const;

  VertexDescPtr gen_slot_desc(int index) const;

  // print Desc info
  friend std::ostream& operator << (std::ostream& os, const VertexDesc& data);

  // debug function
  std::string dump_vertex_data() const;
 private:
  typedef std::vector<int> OffsetIndex;
  void init(const Desc* desc, int desc_count);
  void ReCalc(int strip);
 
  // for debug, we use static array
  static const int32_t kMaxDescCount = 32;
  Desc desc_[kMaxDescCount];
  int vertex_size_;
  int slot_count_;
  OffsetIndex offsets_idx_;
  std::vector<int> slot_index_;
  std::vector<int> slot_stride_;
  std::vector<int> slot_element_;
  DISALLOW_COPY_AND_ASSIGN(VertexDesc);
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
}  // namespace azer
