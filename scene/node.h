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

namespace azer {
class MovableObject;
class RenderSystem;
class Scene;
class SceneNode;
typedef scoped_refptr<SceneNode> SceneNodePtr;

class AZER_EXPORT SceneNode: ublic ::base::RefCounted<SceneNode> {
 public:
  SceneNode();
  ~SceneNode();

  typedef std::vector<SceneNodePtr> SceneNodes;

  
  void set_visible(bool visible) { visible_ = visible;}
  bool is_visible() const { return visible_;}

  void Attach(const RenderableObjectPtr& object);
  void Detach();

  void AddChild(SceneNodePtr child);
  void RemoveChild(SceneNodePtr child);
  bool has_child() const { return !children_.empty();} 
  bool HasAncestor(SceneNode* node) const;

  SceneNode* root() { return root_;}
  SceneNode* parent() { return parent_;}
  const SceneNode* parent() const { return parent_;}
  const SceneNodes& children() const { return children_;}
 protected:
  virtual void OnAttachedToScene();
  bool visible_;

  SceneNode* root_;
  SceneNode* parent_;
  SceneNodes children_;
  RenderableObjectPtr renderable_;
  friend class Scene;
  DISALLOW_COPY_AND_ASSIGN(SceneNode);
};
}  // namespace azer
