#include "azer/render/vertex_layout.h"

#include <sstream>

#include "base/logging.h"
#include "azer/math/math.h"
#include "azer/render/render_system.h"

namespace azer {

VertexDesc::VertexDesc(const Desc* desc, int desc_num)
    : vertex_size_(0),
      slot_count_(0) {
  memset(desc_, 0, sizeof(desc_));
  init(desc, desc_num);
}

VertexDesc::VertexDesc(const Desc* desc) 
    : vertex_size_(0),
      slot_count_(0) {
  memset(desc_, 0, sizeof(desc_));
  int desc_num = 0;
  const Desc* cur = desc;
  while (cur->name[0] != '\0') {
    desc_num++;
    cur++;
  }
  init(desc, desc_num);
}

VertexDesc::~VertexDesc() {
}

namespace {
int calc_vertex_desc_size(const VertexDesc::Desc* desc) {
  // TODO(yanglei), align not support
  return SizeofDataFormat(desc->type);
}
}

void VertexDesc::init(const Desc* desc, int desc_num) {
  DCHECK_EQ(offsets_idx_.size(), 0u);
  uint32_t offset = 0;
  const Desc* cur = desc;
  int cur_slot_count = -1;
  int element_size = 0, prev_element_size = 0;
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

  slot_count_ = static_cast<int>(slot_stride_.size());
  
  CHECK_LT(desc_num, kMaxDescCount);
  memcpy(desc_, desc, sizeof(Desc) * desc_num);
}

std::ostream& operator << (std::ostream& os, const azer::VertexDesc& vertex_data) {
  os << "Vertex Data: [" << std::endl;
  int desc_num = static_cast<int>(vertex_data.offsets_idx_.size());
  for (int i = 0; i < desc_num; ++i) {
    const azer::VertexDesc::Desc& desc = vertex_data.desc_[i];
    os << "\t" << desc.name;
  }
  os << "]" << std::endl;
      
  return os;
}

int VertexDesc::offset(const int index) const {
  DCHECK_LT(index, static_cast<int>(offsets_idx_.size()));
  return offsets_idx_[index];
}

int VertexDesc::slot_count() const {
  return slot_count_;
}

int VertexDesc::stride(int index) const {
  DCHECK(index < static_cast<int>(slot_stride_.size()));
  return slot_stride_[index];
}

int VertexDesc::vertex_size() const {
  return vertex_size_;
}

int VertexDesc::element_slot(int index) const {
  return static_cast<int>(slot_index_[index]);
}

int VertexDesc::element_index_inslot(int index) const {
  int idx = -1;
  int slot = slot_index_[index];
  for (int i = index; i >= 0; --i, ++idx) {
    if (slot_index_[i] != slot)
      break;
  }

  return idx;
}

int VertexDesc::element_count_inslot(int index) const {
  CHECK(index >= 0);
  return slot_element_[index];
}

int VertexDesc::element_count() const {
  return static_cast<int>(slot_index_.size());
}

const VertexDesc::Desc* VertexDesc::descs() const { 
  return desc_; 
}

VertexDescPtr VertexDesc::gen_slot_desc(int sindex) const {
  int element_count = this->element_count_inslot(sindex);
  std::unique_ptr<VertexDesc::Desc[]> descptr(new VertexDesc::Desc[element_count]);
  VertexDesc::Desc* desc = descptr.get();
  for (int i = 0; i < static_cast<int>(slot_index_.size()); i++) {
    if (slot_index_[i] != sindex) {
      continue;
    }
    memcpy(desc, desc_ + i, sizeof(Desc));
    desc->input_slot = 0;
    desc++;
  }
  return VertexDescPtr(new VertexDesc(descptr.get(), element_count));
}
}  // namespace azer
