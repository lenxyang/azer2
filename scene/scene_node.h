#pragma once

#include <map>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "azer/math/math.h"
#include "azer/base/export.h"
#include "azer/render/transform_holder.h"
#include "azer/scene/scene_node_observer.h"

namespace azer {
class Light;
class Mesh;
class Renderer;
class SceneNode;
class SceneNodeData;
class SceneNodeObserver;
typedef scoped_refptr<Light> LightPtr;
typedef scoped_refptr<Mesh> MeshPtr;
typedef scoped_refptr<SceneNode> SceneNodePtr;
typedef scoped_refptr<SceneNodeData> SceneNodeDataPtr;
typedef std::vector<SceneNodePtr> SceneNodes;

enum SceneNodeType {
  kSceneNode,
  kEnvSceneNode,
  kObjectSceneNode,
  kLampSceneNode,
  kCameraSceneNode,
  kTerrainTileSceneNode,
};

class AZER_EXPORT SceneNodeData : public SceneNodeObserver {
 public:
  SceneNodeData(SceneNode* node);
  ~SceneNodeData();

  void reset();
  void AttachMesh(MeshPtr mesh);
  Mesh* GetMesh();

  void AttachLight(Light* light);
  Light* light();

  void SetSceneNode(SceneNode* node);
 private:
  // override SceneNodeObserver
  void OnNodeOrientChanged(SceneNode* node, const Quaternion& prev) override;
  void OnNodeLocationChanged(SceneNode* node, const Vector3& prev) override;
  MeshPtr mesh_;
  LightPtr light_;
  SceneNode* node_;
  DISALLOW_COPY_AND_ASSIGN(SceneNodeData);
};

class AZER_EXPORT SceneNode: public ::base::RefCounted<SceneNode> {
 public:
  SceneNode();
  SceneNode(const std::string& name);
  SceneNode(const std::string& name, SceneNodeType type);
  ~SceneNode();
  
  // attributes
  static SceneNode* Lookup(int32_t id);
  int32_t id() const { return id_;}
  void set_visible(bool visible) { visible_ = visible;}
  bool visible() const { return visible_;}

  void set_draw_bounding_volumn(bool b);
  bool is_draw_bounding_volumn() const;
  void set_pickable(bool pickable) { pickable_ = pickable;}
  bool pickable() const { return pickable_;}
  void set_picked(bool picked) { picked_ = picked;}
  bool picked() const { return picked_;}

  void set_shadow_caster(bool v) {shadow_caster_ = v;}
  bool shadow_caster() const { return shadow_caster_;}

  const Vector3& local_vmin() const { return local_vmin_;}
  const Vector3& local_vmax() const { return local_vmax_;}
  void SetLocalBounds(const Vector3& vmin, const Vector3& vmax);

  SceneNode* AddChild(SceneNode* child);
  void RemoveChild(SceneNode* child);
  bool has_child() const { return !children_.empty();}
  int32_t child_count() const { return static_cast<int32_t>(children_.size());}
  SceneNode* child_at(int32_t index) { return children_.at(index).get();}
  bool HasAncestor(SceneNode* node) const;
  SceneNodes* mutable_children() { return &children_;}

  // absolute path: //level1/level2/level3
  // relative path: level1/level2/level3
  // relative path: ../level1/level2
  SceneNode* GetNode(const std::string& path);
  void CreatePathRecusive(const std::string& path);
  bool AddChildAtPath(const std::string& parent, SceneNode* node);
  SceneNode* RemoveChildAtPath(const std::string& path);

  void set_name(const std::string& name);
  const std::string& name() const { return name_;}
  std::string path() const;

  SceneNode* root();
  SceneNode* parent() { return parent_;}
  const SceneNode* parent() const { return parent_;}
  const SceneNodes& children() const { return children_;}
  SceneNodes& children() { return children_;}

  // tranform
  const TransformHolder& holder() const { return holder_;}
  void SetPosition(const Vector3& pos);
  const Vector3& position() const { return holder().position();}
  Vector3 GetWorldPosition() const;
  
  void SetScale(const Vector3& v);
  const Vector3& scale() const { return holder().scale();}

  void set_orientation(const Quaternion& q);
  const Quaternion& orientation() const { return holder().orientation();}
  void pitch(const Radians angle);
  void pitch(const Degree angle);
  void yaw(const Radians angle);
  void yaw(const Degree angle);
  void roll(const Radians angle);
  void roll(const Degree angle);
  void rotate(const Vector3& axis, const Degree angle);
  void rotate(const Vector3& axis, const Radians angle);

  std::string print_info();

  SceneNodeType type() const;
  void SetNodeType(SceneNodeType type);
  const SceneNodeData* data() const { return data_.get();}
  SceneNodeData* mutable_data() { return data_.get();}

  void set_user_data(void* data) { user_data_ = data;}
  void* user_data() {return user_data_;}
  const void* user_data() const {return user_data_;}
  void AddAttr(const std::string& name, const std::string& value);
  std::string GetAttr(const std::string& name) const;

  void AddObserver(SceneNodeObserver* observer); 
  void RemoveObserver(SceneNodeObserver* observer);
  bool HasObserver(SceneNodeObserver* observer);
 protected:
  void InitMember();
  void BoundsChanged(const Vector3& orgmin, const Vector3& orgmax);
  void ScaleChanged(const Vector3& org_scale);
  void LocationChanged(const Vector3& orgpos);
  void OrientationChanged(const Quaternion& origin_orient);
  void NotifyParentBoundsChanged();
  void UpdateBoundsByChildren();
  TransformHolder* mutable_holder() { return &holder_;}

  SceneNode* GetLocalChild(const std::string& name);
  void print_info(std::string* str, int depth, SceneNode* node);
  // attributes
  bool visible_;
  bool pickable_;
  bool picked_;
  bool shadow_caster_;
  bool draw_bounding_;
  SceneNode* parent_;
  SceneNodes children_;
  std::string name_;
  SceneNodeType type_;
  void* user_data_;
  std::unique_ptr<SceneNodeData> data_;
  TransformHolder holder_;
  std::map<std::string, std::string> attributes_;

  // bounding
  Vector3 local_vmin_;
  Vector3 local_vmax_;
  ::base::ObserverList<SceneNodeObserver> observers_;
  int32_t id_;
  DISALLOW_COPY_AND_ASSIGN(SceneNode);
};

class AZER_EXPORT SceneNodeManager {
 public:
  SceneNodeManager();
  bool Register(SceneNode* node);
  bool Unregister(SceneNode* node);
  SceneNode* Lookup(int32_t id);
 private:
  std::map<int32_t, SceneNode*> dict_;
  DISALLOW_COPY_AND_ASSIGN(SceneNodeManager);
};

AZER_EXPORT const char* SceneNodeName(int32_t type);
AZER_EXPORT Matrix4 GenWorldMatrixForSceneNode(SceneNode* node);
AZER_EXPORT void GetSceneNodeBounds(SceneNode* node, Vector3* vmin, Vector3* vmax);
}  // namespace azer
