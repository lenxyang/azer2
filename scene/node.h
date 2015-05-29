#pragma once

#include <string>
#include <vector>
#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/math/math.h"
#include "azer/base/string.h"
#include "azer/base/export.h"
#include "azer/render/movable.h"
#include "azer/render/renderable_object.h"

namespace azer {
class MovableObject;
class RenderSystem;
class Scene;
class SceneNode;

typedef scoped_refptr<SceneNode> SceneNodePtr;
typedef scoped_refptr<RenderableObject> RenderableObjectPtr;

class AZER_EXPORT SceneNode: public ::base::RefCounted<SceneNode>,
                             public MovableObject,
                             public MovableObject::Delegate {
 public:
  SceneNode();
  explicit SceneNode(const std::string& name);
  ~SceneNode() override;

  typedef std::vector<SceneNodePtr> SceneNodes;
  
  void set_visible(bool visible) { visible_ = visible;}
  bool is_visible() const { return visible_;}

  void Attach(RenderableObjectPtr object);
  void Detach();
  RenderableObjectPtr GetRenderableObject() { return renderable_;} 

  void AddChild(SceneNodePtr child);
  void RemoveChild(SceneNodePtr child);
  bool has_child() const { return !children_.empty();} 
  bool HasAncestor(SceneNode* node) const;

  void set_name(const std::string& name);
  const std::string& name() const { return name_;}

  SceneNode* root() { return root_;}
  SceneNode* parent() { return parent_;}
  const SceneNode* parent() const { return parent_;}
  const SceneNodes& children() const { return children_;}
  SceneNodes& children() { return children_;}

  const Matrix4& GetWorldMatrix() const { return world_;}
  void UpdateWorldMatrixRecusive();

  std::string print_info();
 protected:
  // override from MovableObject::Delegate
  void OnObjectPositionChanged(const Vector3& origin_position) override;
  void OnObjectOrientationChanged(const Quaternion& origin_orientation) override;

  void OnParentNodePositionChanged();
  void OnParentOrientationChanged();
  void UpdateWorldMatrix();

  virtual void OnAttachedToScene();
  void OnPositionChanged();

  void print_info(std::string* str, int depth, SceneNode* node);
  bool visible_;

  SceneNode* root_;
  SceneNode* parent_;
  SceneNodes children_;
  RenderableObjectPtr renderable_;
  Matrix4 world_;
  Vector3 scale_;
  std::string name_;
  friend class Scene;
  DISALLOW_COPY_AND_ASSIGN(SceneNode);
};
}  // namespace azer
