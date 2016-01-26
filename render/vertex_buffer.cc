#include "azer/render/vertex_buffer.h"

#include <sstream>

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
  int32 cur_slot_count = -1;
  int32 element_size = 0, prev_element_size = 0;
  vertex_size_ = 0;
  offsets_idx_.resize(desc_num);
  for (int i = 0; i < desc_num; ++i, ++cur) {
    prev_element_size = element_size;
    element_size = calc_vertex_desc_size(cur);
    DCHECK(cur_slot_count <= cur->input_slot);
    if (cur_slot_count < cur->input_slot) {
      offset = 0;
      cur_slot_count = cur->input_slot;
      slot_stride_.push_back(element_size);
      slot_element_.push_back(1);
    } else {
      offset += prev_element_size;
      slot_element_.back() += 1;
      slot_stride_.back() += (element_size);
    }

    vertex_size_ += element_size;
    offsets_idx_[i] = offset;
    slot_index_.push_back(cur->input_slot);
  }

  slot_count_ = slot_stride_.size();
  
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

int32 VertexDesc::element_slot(int32 index) const {
  return static_cast<int32>(slot_index_[index]);
}

int32 VertexDesc::element_index_inslot(int32 index) const {
  int32 idx = -1;
  int32 slot = slot_index_[index];
  for (int32 i = index; i >= 0; --i, ++idx) {
    if (slot_index_[i] != slot)
      break;
  }

  return idx;
}

int32 VertexDesc::element_count_inslot(int32 index) const {
  CHECK(index >= 0);
  return slot_element_[index];
}

int32 VertexDesc::element_count() const {
  return static_cast<int32>(slot_index_.size());
}

const VertexDesc::Desc* VertexDesc::descs() const { 
  return desc_.get(); 
}

VertexDescPtr VertexDesc::gen_slot_desc(int32 sindex) const {
  int32 element_count = this->element_count_inslot(sindex);
  scoped_ptr<VertexDesc::Desc[]> descptr(new VertexDesc::Desc[element_count]);
  VertexDesc::Desc* desc = descptr.get();
  int32 index = 0;
  for (int32 i = 0; i < static_cast<int32>(slot_index_.size()); i++) {
    if (slot_index_[i] != sindex) {
      continue;
    }
    memcpy(desc, desc_.get() + i, sizeof(Desc));
    desc->input_slot = 0;
    desc++;
  }
  return VertexDescPtr(new VertexDesc(descptr.get(), element_count));
}

// class SlotVertexData
SlotVertexData::SlotVertexData(VertexDesc* desc, int vertex_count)
    : Resource(kVertexData)
    , desc_(desc)
    , vertex_count_(vertex_count) {
  DCHECK(desc->slot_count() == 1);
  int capability = vertex_count_ * stride();
  data_.reset(new uint8[capability]);
}

const VertexDesc* SlotVertexData::vertex_desc() const {
  DCHECK(desc_.get() != NULL);
  return desc_.get();
}

VertexDesc* SlotVertexData::vertex_desc() {
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

const uint8* SlotVertexData::vertex_data_at(int32 index) const {
  int32 pindex = index;
  const VertexDesc::Desc* d = desc_->descs();
  if (d->instance_data_step > 1) {
    pindex = static_cast<int32>(
        (index + d->instance_data_step - 1) / d->instance_data_step);
  }
  return pointer() + desc_->vertex_size() * pindex;
}

uint8* SlotVertexData::vertex_data_at(int32 index) {
  return const_cast<uint8*>(
      const_cast<const SlotVertexData*>(this)->vertex_data_at(index));
}

uint8* SlotVertexData::pointer() {
  return data_.get();
}

const uint8* SlotVertexData::pointer() const {
  return data_.get();
}

int32 SlotVertexData::buffer_size() const {
  return vertex_count_ * stride();
}

int32 SlotVertexData::vertex_count() const {
  return vertex_count_;
}

int32 SlotVertexData::element_count() const {
  DCHECK(desc_.get() != NULL);
  return desc_->element_count();
}

int32 SlotVertexData::stride() const {
  DCHECK(desc_.get() != NULL);
  return desc_->stride(0);
}

// class VertexData
VertexData::VertexData(VertexDesc* desc, int32 vertex_count)
    : desc_(desc),
      vertex_count_(vertex_count) {
  vector_.resize(desc->slot_count());
}

VertexData::~VertexData() {
}

void VertexData::InitSlotFromDesc() {
  DCHECK(static_cast<int32>(vector_.size()) == desc_->slot_count());
  for (int32 i = 0; i < desc_->slot_count(); ++i) {
    VertexDescPtr desc = desc_->gen_slot_desc(i);
    const VertexDesc::Desc* d = desc->descs();
    int32 vertex_count = d->instance_data_step == 0 ? vertex_count_
        : (vertex_count_ + d->instance_data_step - 1) / d->instance_data_step;
    SlotVertexDataPtr data = new SlotVertexData(desc, vertex_count);
    vector_[i] = data;
  }
}

void VertexData::set_slot_vertex_data(SlotVertexData* data, int32 slot_index) {
  vector_[slot_index] = data;
}

SlotVertexData* VertexData::vertex_data_at(int32 index) {
  return vector_[index].get();
}

const VertexDesc* VertexData::vertex_desc() const {
  return desc_.get();
}

VertexDesc* VertexData::vertex_desc() {
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
    , vertex_count_(-1) {
}

VertexBuffer::~VertexBuffer() {}

int32 VertexBuffer::vertex_count() const { return vertex_count_;}
int32 VertexBuffer::buffer_size() const { return vertex_count_;}
int32 VertexBuffer::element_size() const { return element_size_;}
VertexDesc* VertexBuffer::vertex_desc() { return layout_->desc();}
VertexLayout* VertexBuffer::vertex_layout() { return layout_.get();}

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
VertexBufferGroup::VertexBufferGroup(VertexDesc* vdesc)
    : vdesc_(vdesc),
      vertex_count_(-1) {
}

VertexBufferGroup::~VertexBufferGroup() {
}

bool VertexBufferGroup::validate() const {
  return true;
}

VertexBuffer* VertexBufferGroup::vertex_buffer_at(int32 index) {
  DCHECK(index < vertex_buffer_count());
  return vector_[index].get();
}

void VertexBufferGroup::add_vertex_buffer(VertexBuffer* vb) {
  DCHECK(vertex_buffer_count() <= vdesc_->slot_count());
  add_vertex_buffer_at(vb, vertex_buffer_count());
}

void VertexBufferGroup::add_vertex_buffer_at(VertexBuffer* vb, int32 index) {
  DCHECK(vertex_buffer_count() <= vdesc_->slot_count());
  DCHECK(vertex_count_ == -1 || vertex_count_ == vb->vertex_count());
  vertex_count_ = vb->vertex_count();
  vector_.insert(vector_.begin() + index, vb);
  OnVertexBufferChanged();
}

void VertexBufferGroup::remove_vertex_buffer_at(int32 index) {
  vector_.erase(vector_.begin() + index);
  OnVertexBufferChanged();
}

std::string DumpVertexDesc(const VertexDesc* desc) {
  std::stringstream ss;
  for (uint32 i = 0; i < desc->element_count(); ++i) {
    const VertexDesc::Desc* d = desc->descs() + i;
    ss << d->name << d->semantic_index
       << " type: " << GetDataFormatName(d->type)
       <<" slot: " << d->input_slot;
    ss << std::endl;
  }
  return ss.str();
}

}  // namespace azer
