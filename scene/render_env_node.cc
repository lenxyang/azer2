#include "azer/scene/render_env_node.h"

#include <sstream>
#include "base/logging.h"
#include "azer/scene/render_node.h"
#include "azer/scene/scene_node.h"
#include "azer/scene/scene_tree_render.h"

namespace azer {

// class
RenderEnvNodeDelegate::RenderEnvNodeDelegate(RenderEnvNode* envnode)
    : envnode_(envnode) {
}

// class RenderEnvNode
RenderEnvNode::RenderEnvNode() 
    : parent_(NULL),
      delegate_(NULL) {
}

RenderEnvNode::RenderEnvNode(RenderEnvNode* parent) 
    : parent_(parent),
      delegate_(NULL) {
}

RenderEnvNode::~RenderEnvNode() {}
void RenderEnvNode::set_delegate(RenderEnvNodeDelegate* delegate) {
  DCHECK(NULL == delegate_);
  delegate_ = delegate;
}

RenderEnvNode* RenderEnvNode::root() {
  RenderEnvNode* cur = this;
  while (cur->parent()) {
    cur = cur->parent();
  }
  return cur;
}

RenderEnvNode* RenderEnvNode::parent() {
  return parent_;
}

int32_t RenderEnvNode::child_count() const {
  return static_cast<int32_t>(children_.size());
}

RenderEnvNode* RenderEnvNode::child_at(int32_t index) {
  return children_.at(index).get();
}

void RenderEnvNode::AddChild(RenderEnvNode* child) {
  DCHECK(!Contains(child));
  child->parent_ = this;
  children_.push_back(child);
}

bool RenderEnvNode::RemoveChild(RenderEnvNode* child) {
  int32_t index = GetIndexOf(child);
  if (index > 0) {
    children_.erase(children_.begin() + index);
    child->parent_ = NULL;
    return true;
  } else {
    return false;
  }
}

bool RenderEnvNode::Contains(RenderEnvNode* child) const {
  RenderEnvNode* cur = child;
  while (cur) {
    if (cur == this)
      return true;
    cur = cur->parent();
  }
  return false;
}

int32_t RenderEnvNode::GetIndexOf(RenderEnvNode* child) const {
  int32_t index = 0;
  for (auto iter = children_.begin(); iter != children_.end(); ++iter, ++index) {
    if (iter->get() == child)
      return index;
  }

  return -1;
}

std::string RenderEnvNode::DumpTree() const {
  return std::move(DumpNode(this, 0));
}

std::string RenderEnvNode::DumpNode(const RenderEnvNode* node,
                                    int depth) const {
  std::stringstream ss;
  ss << std::string(depth, ' ') << "RenderEnvNode[" << (void*)node << "]"
     << std::endl;
  for (auto iter = node->children_.begin(); iter != node->children_.end(); ++iter) {
    ss << std::move(DumpNode(iter->get(), depth + 2));
  }
  return ss.str();
}

void RenderEnvNode::UpdateRecusive(const FrameArgs& args) {
  delegate()->OnUpdateNode(args);
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    (*iter)->UpdateRecusive(args);
  }
}

}  // namespace azer
