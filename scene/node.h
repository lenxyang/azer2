#pragma once

#include <string>
#include <vector>
#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/math/math.h"
#include "azer/base/tree_node.h"
#include "azer/base/string.h"
#include "azer/base/hierarchy_transform.h"
#include "azer/base/export.h"
#include "azer/render/movable.h"
#include "azer/render/renderable_object.h"

namespace azer {
class MovableObject;
class RenderSystem;
class Scene;
class SceneNode;
typedef scoped_refptr<SceneNode> SceneNodePtr;

class AZER_EXPORT SceneNode : public HierarchyTransform<SceneNode>,
                              public MovableObject::Delegate,
                              public TreeNode<SceneNode>::Delegate,
                              public ::base::RefCounted<SceneNode> {
 public:
  ~SceneNode() override;
  struct NodeInfo {
    StringType name;
    StringType attach_object;
    Vector3 position;
    Vector3 scale;
    Quaternion orientation;
    Type type;
    NodeInfo()
        : scale(1.0f, 1.0f, 1.0f) {
    }
  };

  typedef std::shared_ptr<NodeInfo> NodeInfoPtr;

  Scene* root() { return root_;}
  void set_visible(bool visible) { visible_ = visible;}
  bool is_visible() const { return visible_;}

  void Attach(const RenderableObjectPtr& object);
  void Detach();
 protected:
  SceneNode(SceneNode* parent);

  // override from MovableObject::Delegate
  void OnObjectPositionChanged(const Vector3& origin_position) override;
  void OnObjectOrientationChanged(const Quaternion& origin_orientation) override;

  // override from TreeNode<T>::Delegate
  void OnChildAdded(SceneNode* child) override;
  void OnChildRemoved(SceneNode* child) override;

  bool visible_;
  SceneNode* root_;
  RenderableObjectPtr renderable_;

  friend class Scene;
  DISALLOW_COPY_AND_ASSIGN(SceneNode);
};
}  // namespace azer
