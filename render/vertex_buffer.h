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
class VertexData;
class VertexDesc;
typedef scoped_refptr<VertexData> VertexDataPtr;
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
  int32 stride() const;
  int32 element_num() const;
  const Desc* descs() const;

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
  std::unique_ptr<Desc[]> desc_;
  int32 vertex_size_;
  int32 slot_count_;

  typedef std::vector<int32> OffsetIndex;
  OffsetIndex offsets_idx_;

  DISALLOW_COPY_AND_ASSIGN(VertexDesc);
};

class AZER_EXPORT VertexData : public Resource {
 public:
  enum MemoryMode {
    kMainMemory,
    kGPUMemr,
  };

  VertexData(const VertexDescPtr& desc, int vertex_num);

  /**
   * 枚举方法
   * 如果对 next 的参数为 NULL， 它将返回第一个定点
   * 当 next() 返回 NULL 时， 表明已经到达最后一个定点
   */
  uint8* next(uint8* current) const;
  uint8* pointer() const;
  void CopyFrom(uint8* data, uint32 size);

  int32 vertex_num() const;
  int32 element_num() const;

  int32 stride() const;

  const VertexDesc* desc() const;
  VertexDesc* desc();

  /**
   * whole buffer sizef of vertex data
   */
  int32 buffer_size() const {
    return vertex_num() * stride();
  }

  void reset() { data_.reset();}
 private:
  int32 vertex_num_;
  std::unique_ptr<uint8[]> data_;
  VertexDescPtr desc_ptr_;
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
