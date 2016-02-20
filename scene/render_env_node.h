#pragma once

#include <deque>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/ref_counted.h"
#include "azer/effect/light.h"
#include "azer/render/render.h"
#include "azer/scene/scene_node_traverse.h"
#include "azer/scene/scene_node.h"

namespace azer {
class RenderNode;
class RenderEnvNode;
class RenderEnvNodeDelegate;
typedef scoped_refptr<RenderEnvNode> RenderEnvNodePtr;
typedef scoped_refptr<RenderEnvNodeDelegate> RenderEnvNodeDelegatePtr;

class RenderEnvNodeDelegate : public EffectParamsProvider {
 public:
  explicit RenderEnvNodeDelegate(RenderEnvNode* envnode);

  RenderEnvNode* node() { return envnode_;}
  const RenderEnvNode* node() const { return envnode_;}

  virtual void Init(SceneNode* node, RenderNode* render_node) = 0;
  virtual void OnUpdateNode(const FrameArgs& args) = 0;
 private:
  RenderEnvNode* envnode_;
  DISALLOW_COPY_AND_ASSIGN(RenderEnvNodeDelegate);
};

class FakeEnvNode : RenderEnvNodeDelegate {
 public:
  FakeEnvNode(RenderEnvNode* envnode) :  RenderEnvNodeDelegate(envnode) {}
  void Init(SceneNode* node, RenderNode* render_node) override {}
  void OnUpdateNode(const FrameArgs& args) override {}
};

class RenderEnvNode : public ::base::RefCounted<RenderEnvNode> {
 public:
  RenderEnvNode();
  explicit RenderEnvNode(RenderEnvNode* parent);
  ~RenderEnvNode();

  void set_delegate(RenderEnvNodeDelegate* delegate);
  RenderEnvNodeDelegate* delegate() { return delegate_.get();}
  const RenderEnvNodeDelegate* delegate() const { return delegate_.get();}
  
  RenderEnvNode* root();
  const RenderEnvNode* root() const {
    return const_cast<const RenderEnvNode*>(
        const_cast<RenderEnvNode*>(this)->root());
  }
  RenderEnvNode* parent();
  const RenderEnvNode* parent() const {
    return const_cast<const RenderEnvNode*>(
        const_cast<RenderEnvNode*>(this)->parent());
  }

  void UpdateRecusive(const FrameArgs& args);
  int32 child_count() const;
  RenderEnvNode* child_at(int32 index);
  void AddChild(RenderEnvNode* child);
  bool RemoveChild(RenderEnvNode* child);
  bool Contains(RenderEnvNode* child) const;
  int32 GetIndexOf(RenderEnvNode* child) const;
  std::string DumpTree() const;
  std::string DumpNode(const RenderEnvNode* node, int32 dump) const;
 private:
  RenderEnvNode* parent_;
  std::vector<RenderEnvNodePtr> children_;
  RenderEnvNodeDelegatePtr delegate_;
  DISALLOW_COPY_AND_ASSIGN(RenderEnvNode);
};

}  // namespace azer
