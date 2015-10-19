#include "azer/render/vertex_buffer.h"

#include "base/logging.h"

#include "azer/math/math.h"
#include "azer/render/render_system.h"

namespace azer {

VertexDesc::VertexDesc(const Desc* desc, int desc_num)
    : Resource(kVertexDesc),
      vertex_size_(0),
      slot_count_(0) {
  init(desc, desc_num);
}

VertexDesc::~VertexDesc() {
}

namespace {
int32 calc_vertex_desc_size(const VertexDesc::Desc* desc) {
  // TODO(yanglei), align not support
  return VertexTypeSize(desc->type);
}
}

void VertexDesc::init(const Desc* desc, int desc_num) {
  DCHECK_EQ(offsets_idx_.size(), 0u);
  uint32 offset = 0;
  const Desc* cur = desc;
  int32 cur_slot_count = 0;
  int32 slot_element_size = 0;
  offsets_idx_.resize(desc_num);
  slot_element_.push_back(0);
  for (int i = 0; i < desc_num; ++i, ++cur) {
    offsets_idx_[i] = offset;
    int32 element_size = calc_vertex_desc_size(cur);
    offset += element_size;
    slot_index_.push_back(cur->input_slot);
    DCHECK(cur_slot_count <= cur->input_slot);
    if (cur_slot_count < cur->input_slot) {
      cur_slot_count = cur->input_slot;
      slot_stride_.push_back(slot_element_size);
      slot_element_size = element_size;
    } else {
      slot_element_size += element_size;
      slot_element_.back() += 1;
    }
  }

  slot_element_.back() += 1;
  slot_stride_.push_back(slot_element_size);
  slot_count_ = slot_stride_.size();
  vertex_size_ = offset;
  desc_.reset(new Desc[desc_num]);
  memcpy(desc_.get(), desc, sizeof(Desc) * desc_num);
}

std::ostream& operator << (std::ostream& os, const azer::VertexDesc& vertex_data) {
  os << "Vertex Data: [" << std::endl;
  int desc_num = static_cast<int>(vertex_data.offsets_idx_.size());
  for (int i = 0; i < desc_num; ++i) {
    const azer::VertexDesc::Desc& desc = vertex_data.desc_.get()[i];
    os << "\t" << desc.name;
  }
  os << "]" << std::endl;
      
  return os;
}

int32 VertexDesc::offset(const int32 index) const {
  DCHECK_LT(index, offsets_idx_.size());
  return offsets_idx_[index];
}

int32 VertexDesc::slot_count() const {
  return slot_count_;
}

int32 VertexDesc::stride(int32 index) const {
  DCHECK(index < static_cast<int32>(slot_stride_.size()));
  return slot_stride_[index];
}

int32 VertexDesc::vertex_size() const {
  return vertex_size_;
}

int32 VertexDesc::element_num(int32 index) const {
  return slot_element_[index];
}

const VertexDesc::Desc* VertexDesc::descs() const { 
  return desc_.get(); 
}

VertexDescPtr VertexDesc::gen_slot_desc(int32 sindex) const {
  int32 element_num = this->element_num(sindex);
  scoped_ptr<VertexDesc::Desc[]> descptr(new VertexDesc::Desc[element_num]);
  VertexDesc::Desc* desc = descptr.get();
  int32 index = 0;
  for (int32 i = 0; i < static_cast<int32>(slot_index_.size()); i) {
    if (slot_index_[i] != sindex) {
      continue;
    }
    memcpy(desc, desc_.get() + i, sizeof(Desc));
    desc->input_slot = 0;
    desc++;
  }
  return VertexDescPtr(new VertexDesc(descptr.get(), element_num));
}

// class SlotVertexData
SlotVertexData::SlotVertexData(const VertexDescPtr& desc, int vertex_num)
    : Resource(kVertexData)
    , desc_(desc)
    , vertex_num_(vertex_num) {
  DCHECK(desc->slot_count() == 1);
  int capability = vertex_num_ * stride();
  data_.reset(new uint8[capability]);
}

const VertexDesc* SlotVertexData::desc() const {
  DCHECK(desc_.get() != NULL);
  return desc_.get();
}

VertexDesc* SlotVertexData::desc() {
  DCHECK(desc_.get() != NULL);
  return desc_.get();
}

const uint8* SlotVertexData::next(const uint8* cur) const {
  DCHECK(cur != NULL);
  DCHECK_EQ((cur - pointer()) % stride(), 0);
  const uint8* next_ptr = cur + stride();
  return (next_ptr < pointer() + buffer_size()) ? next_ptr : NULL;
}

uint8* SlotVertexData::next(const uint8* cur) {
  return const_cast<uint8*>(
      const_cast<const SlotVertexData*>(this)->next(cur));
}

uint8* SlotVertexData::pointer() {
  return data_.get();
}

const uint8* SlotVertexData::pointer() const {
  return data_.get();
}

int32 SlotVertexData::buffer_size() const {
  return vertex_num_ * stride();
}

int32 SlotVertexData::vertex_num() const {
  return vertex_num_;
}

int32 SlotVertexData::element_num() const {
  DCHECK(desc_.get() != NULL);
  return desc_->element_num(0);
}

int32 SlotVertexData::stride() const {
  DCHECK(desc_.get() != NULL);
  return desc_->stride(0);
}

// class VertexData
VertexData::VertexData(const VertexDescPtr& desc, int32 vertex_num)
    : desc_(desc),
      vertex_num_(vertex_num) {
}

VertexData::~VertexData() {
}

void VertexData::InitSlotFromDesc() {
  DCHECK(vector_.size() == 0u);
  for (int32 i = 0; i < desc_->slot_count(); ++i) {
    VertexDescPtr desc = desc_->gen_slot_desc(i);
    const VertexDesc::Desc* d = desc->descs();
    int32 vertex_count = d->instance_data_step == 0 ? vertex_num_
        : (vertex_num_ + d->instance_data_step - 1) / d->instance_data_step;
    SlotVertexDataPtr data = new SlotVertexData(VertexDescPtr(desc.get()),
                                                vertex_count);
    vector_[i] = data;
  }
}

void VertexData::set_slot_vertex_data(SlotVertexDataPtr data, int32 slot_index) {
  vector_[slot_index] = data;
}

SlotVertexData* VertexData::vertex_data_at(int32 index) {
  return vector_[index].get();
}

const VertexDesc* VertexData::desc() const {
  return desc_.get();
}

// class VertexBuffer
VertexBuffer::Options::Options()
    : usage(GraphicBuffer::kDefault)
    , cpu_access(kCPUNoAccess) {
  memset(name, 0, sizeof(name));
}

VertexBuffer::VertexBuffer(const Options &opt)
    : options_(opt)
    , element_size_(-1)
    , buffer_size_(-1)
    , vertex_num_(-1) {
}

VertexBuffer::~VertexBuffer() {
}

int32 VertexTypeSize(DataFormat type) {
  switch (type) {
    case kScalar: return sizeof(float);
    case kVec2: return sizeof(azer::Vector2);
    case kVec3: return sizeof(azer::Vector3);
    case kVec4: return sizeof(azer::Vector4);
    case kInt: return sizeof(int);
    case kIntVec2: return sizeof(int) * 2;
    case kIntVec3: return sizeof(int) * 3;
    case kIntVec4: return sizeof(int) * 4;
    case kUint: return sizeof(uint32);
    case kUintVec2: return sizeof(uint32) * 2;
    case kUintVec3: return sizeof(uint32) * 3;
    case kUintVec4: return sizeof(uint32) * 4;
    case kBool: return sizeof(bool);
    case kBoolVec2: return sizeof(bool) * 2;
    case kBoolVec3: return sizeof(bool) * 3;
    case kBoolVec4: return sizeof(bool) * 4;
    case kRGBA: return sizeof(azer::Vector4);
    default:
      DCHECK(false) <<" No VertexDesc::Type: " << type;
      return 0;
  }
}


// class VertexBufferGroup
VertexBufferGroup::VertexBufferGroup(VertexDescPtr vdesc)
    : vdesc_(vdesc) {
}

VertexBufferGroup::~VertexBufferGroup() {
}

int32 VertexBufferGroup::vertex_buffer_count() const {
  return static_cast<int32>(vector_.size());
}

VertexBuffer* VertexBufferGroup::vertex_buffer_at(int32 index) {
  DCHECK(index < vertex_buffer_count());
  return vector_[index].get();
}

void VertexBufferGroup::add_vertex_buffer(VertexBufferPtr vb) {
  DCHECK(vertex_buffer_count() + 1 <= vdesc_->slot_count());
  add_vertex_buffer_at(vb, vertex_buffer_count() - 1);
}

void VertexBufferGroup::add_vertex_buffer_at(VertexBufferPtr vb, int32 index) {
  vector_.insert(vector_.begin() + index, vb);
}

void VertexBufferGroup::remove_vertex_buffer_at(int32 index) {
  vector_.erase(vector_.begin() + index);
}
}  // namespace azer
