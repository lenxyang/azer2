#include "azer/render/vertex_buffer.h"

#include <sstream>

#include "base/logging.h"
#include "azer/math/math.h"
#include "azer/render/render_system.h"

namespace azer {

// class SlotVertexData
SlotVertexData::SlotVertexData(VertexDesc* desc, int vertex_count)
    : desc_(desc),
      vertex_count_(vertex_count) {
  DCHECK(desc->slot_count() == 1);
  int capability = vertex_count_ * stride();
  data_.reset(new uint8_t[capability]);
}

void SlotVertexData::extend(int count) {
  int capability = (count + vertex_count_) * stride();
  uint8_t* data(new uint8_t[capability]);
  memcpy(data, data_.get(), vertex_count_ * stride());
  data_.reset(data);
  vertex_count_ += count;
}

const VertexDesc* SlotVertexData::vertex_desc() const {
  DCHECK(desc_.get() != NULL);
  return desc_.get();
}

VertexDesc* SlotVertexData::vertex_desc() {
  DCHECK(desc_.get() != NULL);
  return desc_.get();
}

const uint8_t* SlotVertexData::next(const uint8_t* cur) const {
  DCHECK(cur != NULL);
  DCHECK_EQ((cur - pointer()) % stride(), 0);
  const uint8_t* next_ptr = cur + stride();
  return (next_ptr < pointer() + buffer_size()) ? next_ptr : NULL;
}

uint8_t* SlotVertexData::next(const uint8_t* cur) {
  return const_cast<uint8_t*>(
      const_cast<const SlotVertexData*>(this)->next(cur));
}

const uint8_t* SlotVertexData::vertex_data_at(int index) const {
  int pindex = index;
  return pointer() + desc_->vertex_size() * pindex;
}

uint8_t* SlotVertexData::vertex_data_at(int index) {
  return const_cast<uint8_t*>(
      const_cast<const SlotVertexData*>(this)->vertex_data_at(index));
}

uint8_t* SlotVertexData::pointer() {
  return data_.get();
}

const uint8_t* SlotVertexData::pointer() const {
  return data_.get();
}

int SlotVertexData::buffer_size() const {
  return vertex_count_ * stride();
}

int SlotVertexData::vertex_count() const {
  return vertex_count_;
}

int SlotVertexData::element_count() const {
  DCHECK(desc_.get() != NULL);
  return desc_->element_count();
}

int SlotVertexData::stride() const {
  DCHECK(desc_.get() != NULL);
  return desc_->stride(0);
}

// class VertexData
VertexData::VertexData(VertexDesc* desc)
    : desc_(desc),
      vertex_count_(-1) {
  vector_.resize(desc->slot_count());
}

VertexData::VertexData(VertexDesc* desc, int vertex_count)
    : desc_(desc),
      vertex_count_(vertex_count) {
  vector_.resize(desc->slot_count());
  InitSlotFromDesc();
}

VertexData::~VertexData() {
  vector_.clear();
}

void VertexData::extend(int count) {
  for (auto iter = vector_.begin(); iter != vector_.end(); ++iter) {
    (*iter)->extend(count);
  }
  vertex_count_ += count;
}

void VertexData::InitSlotFromDesc() {
  DCHECK(static_cast<int>(vector_.size()) == desc_->slot_count());
  for (int i = 0; i < desc_->slot_count(); ++i) {
    VertexDescPtr desc = desc_->gen_slot_desc(i);
    int vertex_count = vertex_count_;
    SlotVertexDataPtr data = new SlotVertexData(desc.get(), vertex_count);
    vector_[i] = data;
  }
}

void VertexData::set_slot_vertex_data(SlotVertexData* data, int slot_index) {
  vector_[slot_index] = data;
  if (slot_index == 0)
    vertex_count_ = data->vertex_count();
}

SlotVertexData* VertexData::vertex_data_at(int index) {
  return vector_[index].get();
}

const VertexDesc* VertexData::vertex_desc() const {
  return desc_.get();
}

VertexDesc* VertexData::vertex_desc() {
  return desc_.get();
}

VertexBuffer::VertexBuffer(SlotVertexData* vdata)
    : element_size_(vdata->stride()),
      buffer_size_(vdata->buffer_size()),
      vertex_count_(vdata->vertex_count()) {
  GpuResOptions resopt;
  resopt.type = GpuResType::kVertexBuffer;
  resopt.target = kBindTargetVertexBuffer;
  InitGpuBuffer(resopt, vdata);
}

VertexBuffer::VertexBuffer(const GpuResOptions &resopt, SlotVertexData* vdata)
    : element_size_(vdata->stride()),
      buffer_size_(vdata->buffer_size()),
      vertex_count_(vdata->vertex_count()) {
  DCHECK(resopt.type == GpuResType::kVertexBuffer);
  InitGpuBuffer(resopt, vdata);
}

VertexBuffer::~VertexBuffer() {}

void VertexBuffer::InitGpuBuffer(const GpuResOptions& options,
                                 SlotVertexData* vdata) {
  RenderSystem* rs = RenderSystem::Current();
  CHECK(rs) << "RenderSystem Not Initialized";
  layout_ = rs->CreateVertexLayout(vdata->vertex_desc());
  gpu_buffer_ = rs->CreateBuffer(options, 1, vdata->buffer_size(), vdata->pointer());
  CHECK(gpu_buffer_.get()) << "Initializer GpuBuffer for ShaderParam Failed";
}

int VertexBuffer::vertex_count() const { return vertex_count_;}
int VertexBuffer::buffer_size() const { return vertex_count_;}
int VertexBuffer::element_size() const { return element_size_;}
VertexLayout* VertexBuffer::vertex_layout() { return layout_.get();}
const VertexDesc* VertexBuffer::vertex_desc() const {
  CHECK(layout_.get() != NULL);
  return layout_->vertex_desc();
}
VertexDesc* VertexBuffer::vertex_desc() {
  CHECK(layout_.get() != NULL);
  return layout_->vertex_desc();
}


// class VertexBufferGroup
VertexBufferGroup::VertexBufferGroup(VertexDesc* vdesc)
    : vdesc_(vdesc),
      vertex_count_(-1) {
  RenderSystem* rs = RenderSystem::Current();
  CHECK(rs) << "RenderSystem Not Initialized";
  layout_ = rs->CreateVertexLayout(vdesc);
}

VertexBufferGroup::VertexBufferGroup(VertexData* vdata)
    : vdesc_(vdata->vertex_desc()),
      vertex_count_(vdata->vertex_count()) {
  RenderSystem* rs = RenderSystem::Current();
  CHECK(rs) << "RenderSystem Not Initialized";
  layout_ = rs->CreateVertexLayout(vdata->vertex_desc());
  int slot_count = vdata->vertex_desc()->slot_count();
  for (int i = 0; i < slot_count; ++i) {
    SlotVertexData* slot_data = vdata->vertex_data_at(i);
    VertexBufferPtr vb(new VertexBuffer(slot_data));
    add_vertex_buffer(vb.get());
  }
}

VertexBufferGroup::~VertexBufferGroup() {
}

bool VertexBufferGroup::validate() const {
  return true;
}

void VertexBufferGroup::OnVertexBufferChanged() {
  for (int i = 0; i < vertex_buffer_count(); ++i) {
    VertexBuffer* vb = vertex_buffer_at(i);
    handle_[i] = vb->gpu_buffer()->native_handle();
    stride_[i] = vb->element_size();
    offset_[i] = 0;
  }
}

VertexBuffer* VertexBufferGroup::vertex_buffer_at(int index) {
  DCHECK(index < vertex_buffer_count());
  return vector_[index].get();
}

void VertexBufferGroup::add_vertex_buffer(VertexBuffer* vb) {
  DCHECK(vertex_buffer_count() <= vdesc_->slot_count());
  add_vertex_buffer_at(vb, vertex_buffer_count());
}

void VertexBufferGroup::add_vertex_buffer_at(VertexBuffer* vb, int index) {
  DCHECK(vertex_buffer_count() <= vdesc_->slot_count());
  DCHECK(vertex_count_ == -1 || vb->vertex_desc()->descs()[0].instance_data_step > 0
     || vertex_count_ == vb->vertex_count());
  vertex_count_ = vb->vertex_count();
  vector_.insert(vector_.begin() + index, vb);
  OnVertexBufferChanged();
}

void VertexBufferGroup::remove_vertex_buffer_at(int index) {
  vector_.erase(vector_.begin() + index);
  OnVertexBufferChanged();
}

const VertexDesc* VertexBufferGroup::vertex_desc() const {
  return vertex_layout()->vertex_desc();
}

VertexDesc* VertexBufferGroup::vertex_desc() {
  return vertex_layout()->vertex_desc();
}

const VertexLayout* VertexBufferGroup::vertex_layout() const { 
  return layout_.get(); 
}

VertexLayout* VertexBufferGroup::vertex_layout() { 
  return layout_.get(); 
}

std::vector<VertexBufferPtr>* VertexBufferGroup::get_vb_vector() { 
  return &vector_; 
}

std::string DumpVertexDesc(const VertexDesc* desc) {
  std::stringstream ss;
  for (int i = 0; i < desc->element_count(); ++i) {
    const VertexDesc::Desc* d = desc->descs() + i;
    ss << d->name << d->semantic_index
       << " type: " << GetDataFormatName(d->type)
       << " slot: " << d->input_slot;
    ss << std::endl;
  }
  return ss.str();
}
}  // namespace azer
