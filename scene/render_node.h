#pragma once

#include <deque>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/effect/light.h"
#include "azer/effect/effect_params_provider.h"
#include "azer/scene/scene_node_traverse.h"
#include "azer/scene/scene_node.h"

namespace azer {
class Camera;
class FrameArgs;
class RenderEnvNodeDelegate;
class RenderEnvNode;
class RenderNode;
typedef scoped_refptr<RenderEnvNode> RenderEnvNodePtr;
typedef scoped_refptr<RenderEnvNodeDelegate> RenderEnvNodeDelegatePtr;
typedef scoped_refptr<RenderNode> RenderNodePtr;

class AZER_EXPORT RenderNodeDelegate {
 public:
  explicit RenderNodeDelegate(RenderNode* node);
  virtual void Update(const FrameArgs& args) = 0;
  virtual void Render(Renderer* renderer) = 0;
  SceneNode* GetSceneNode();
  const SceneNode* GetSceneNode() const;
  RenderNode* GetRenderNode();
  const RenderNode* GetRenderNode() const;
 protected:
  RenderNode* node_;
  DISALLOW_COPY_AND_ASSIGN(RenderNodeDelegate);
};

class AZER_EXPORT RenderNode : public EffectParamsProvider {
 public:
  explicit RenderNode(SceneNode* node);
  virtual ~RenderNode();
  void SetDelegate(scoped_ptr<RenderNodeDelegate> delegate);

  SceneNode* GetSceneNode() { return node_;}
  const SceneNode* GetSceneNode() const { return node_;}
  RenderEnvNode* GetEnvNode() { return envnode_;}
  const RenderEnvNode* GetEnvNode() const { return envnode_;}
  void  SetEnvNode(RenderEnvNode* node);

  const Matrix4& GetWorld() const { return world_;}
  const Matrix4& GetPVW() const { return pvw_;}
  const Matrix4& GetPV() const { return pv_;}
  const Camera* camera() const;
  void SetCamera(const Camera* camera);

  // virtual function;
  bool Init();
  void Update(const FrameArgs& args);
  void Render(Renderer* renderer);

  RenderNode* root();
  const RenderNode* root() const {
    return const_cast<const RenderNode*>(
        const_cast<RenderNode*>(this)->root());
  }
  RenderNode* parent();
  const RenderNode* parent() const {
    return const_cast<const RenderNode*>(
        const_cast<RenderNode*>(this)->parent());
  }
  int32_t child_count() const;
  RenderNode* child_at(int32_t index);
  void AddChild(RenderNode* child);
  bool RemoveChild(RenderNode* child);
  bool Contains(RenderNode* child) const;
  int32_t GetIndexOf(RenderNode* child) const;
  const std::vector<RenderNodePtr>& children() const { return children_;}
  std::string DumpTree() const;
  std::string DumpNode(const RenderNode* node, int32_t depth) const;
 protected:
  void CalcParams(const FrameArgs& args);
  RenderNode* parent_;
  std::vector<RenderNodePtr> children_;

  SceneNode* node_;
  scoped_ptr<RenderNodeDelegate> delegate_;
  RenderEnvNodePtr envnode_;
  const Camera* camera_;
  Matrix4 world_;
  Matrix4 pvw_;
  Matrix4 pv_;
  DISALLOW_COPY_AND_ASSIGN(RenderNode);
};

class AZER_EXPORT RenderTreeBuilderDelegate {
 public:
  virtual bool NeedRenderNode(SceneNode* node) = 0;
  virtual bool NeedRenderEnvNode(SceneNode* node);
  virtual scoped_ptr<RenderNodeDelegate> CreateRenderDelegate(RenderNode* n) = 0;
  virtual RenderEnvNodeDelegatePtr CreateEnvDelegate(RenderEnvNode* n) = 0;
};

class AZER_EXPORT RenderTreeBuilder : public SceneNodeTraverseDelegate {
 public:
  explicit RenderTreeBuilder(RenderTreeBuilderDelegate* delegate);
  ~RenderTreeBuilder();

  RenderNodePtr Build(SceneNode* node, const Camera* camera);

  // override from SceneNodeTraverseDelegate
  void OnTraverseBegin(SceneNode* root) override;
  bool OnTraverseNodeEnter(SceneNode* node) override;
  void OnTraverseNodeExit(SceneNode* node) override;
  void OnTraverseEnd() override;
 private:
  void UpdateNodeWorld(SceneNode* node);
  RenderNode* cur_;
  RenderTreeBuilderDelegate* delegate_;
  DISALLOW_COPY_AND_ASSIGN(RenderTreeBuilder);
};
}  // namespace azer
