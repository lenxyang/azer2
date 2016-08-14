#pragma once

#include <memory>
#include <string.h>
#include <vector>

#include "azer/base/export.h"
#include "azer/render/common.h"
#include "azer/render/gpu_buffer.h"
#include "azer/render/vertex_layout.h"
#include "azer/render/native_types.h"

namespace azer {

class Renderer;
class RenderSystem;
class SlotVertexData;
class VertexData;
class VertexBuffer;
class VertexBufferGroup;
typedef scoped_refptr<SlotVertexData> SlotVertexDataPtr;
typedef scoped_refptr<VertexData> VertexDataPtr;
typedef scoped_refptr<VertexBuffer> VertexBufferPtr;
typedef scoped_refptr<VertexBufferGroup> VertexBufferGroupPtr;

class AZER_EXPORT SlotVertexData : public ::base::RefCounted<SlotVertexData> {
 public:
  SlotVertexData(VertexDesc* desc, int vertex_count);
  void extend(int count);

  uint8_t* next(const uint8_t* cur);
  const uint8_t* next(const uint8_t* cur) const;

  uint8_t* pointer();
  const uint8_t* pointer() const;

  uint8_t* vertex_data_at(int index);
  const uint8_t* vertex_data_at(int index) const;
  int buffer_size() const;
  int vertex_count() const;
  int element_count() const;
  int stride() const;

  const VertexDesc* vertex_desc() const;
  VertexDesc* vertex_desc();
 private:
  std::unique_ptr<uint8_t[]> data_;
  int vertex_count_;
  VertexDescPtr desc_;
  DISALLOW_COPY_AND_ASSIGN(SlotVertexData);
};

class AZER_EXPORT VertexData : public ::base::RefCounted<VertexData> {
 public:
  VertexData(VertexDesc* desc);
  VertexData(VertexDesc* desc, int vertex_count);
  ~VertexData();

  void extend(int count);
  void set_slot_vertex_data(SlotVertexData* data, int slot_index);
  SlotVertexData* vertex_data_at(int index);

  int vertex_count() const { return vertex_count_;}
  const VertexDesc* vertex_desc() const;
  VertexDesc* vertex_desc();
 private:
  void InitSlotFromDesc();
  VertexDescPtr desc_;
  int vertex_count_;
  std::vector<SlotVertexDataPtr> vector_;
  DISALLOW_COPY_AND_ASSIGN(VertexData);
};

class AZER_EXPORT VertexBuffer : public ::base::RefCounted<VertexBuffer> {
 public:
  explicit VertexBuffer(SlotVertexData* vdata);
  VertexBuffer(const GpuResOptions &opt, SlotVertexData* vdata);
  virtual ~VertexBuffer();

  int vertex_count() const;
  int buffer_size() const;
  int element_size() const;
  VertexDesc* vertex_desc();
  const VertexDesc* vertex_desc() const;
  VertexLayout* vertex_layout();
  GpuBuffer* gpu_buffer() { return gpu_buffer_.get();}
 protected:
  void InitGpuBuffer(const GpuResOptions& options, SlotVertexData* vdata);

  VertexLayoutPtr layout_;
  scoped_refptr<GpuBuffer> gpu_buffer_;
  int element_size_;
  int buffer_size_;
  int vertex_count_;
  DISALLOW_COPY_AND_ASSIGN(VertexBuffer);
};

class AZER_EXPORT VertexBufferGroup : public ::base::RefCounted<VertexBufferGroup> {
public:
  explicit VertexBufferGroup(VertexDesc* vdesc, int slot_count);
  explicit VertexBufferGroup(VertexData* vdata);
  ~VertexBufferGroup();

  bool validate() const;
  int vertex_count() const { return vertex_count_;}
  int vertex_buffer_count() const { return static_cast<int>(vector_.size());}
  VertexBuffer* vertex_buffer_at(int index);
  void add_vertex_buffer(VertexBuffer* vb);
  void add_vertex_buffer_at(VertexBuffer* vb, int index);
  void remove_vertex_buffer_at(int index);
  std::vector<VertexBufferPtr>* get_vb_vector();

  VertexDesc* vertex_desc();
  const VertexDesc* vertex_desc() const;
  VertexLayout* vertex_layout();
  const VertexLayout* vertex_layout() const;

  NativeGpuResourceHandle* handle() { return handle_;}
  uint32_t* stride() { return stride_;}
  uint32_t* offset() { return offset_;}
 protected:
  void OnVertexBufferChanged();
  VertexLayoutPtr layout_;
 private:
  VertexDescPtr vdesc_;
  std::vector<VertexBufferPtr> vector_;
  int vertex_count_;

  static const int kMaxVertexBuffer = 16;
  NativeGpuResourceHandle handle_[kMaxVertexBuffer];
  uint32_t stride_[kMaxVertexBuffer];
  uint32_t offset_[kMaxVertexBuffer];
  
  DISALLOW_COPY_AND_ASSIGN(VertexBufferGroup);
};

AZER_EXPORT int VertexTypeSize(DataFormat type);
AZER_EXPORT std::string DumpVertexDesc(const VertexDesc* desc);
}  // namespace azer
