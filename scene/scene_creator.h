#pragma once

#include <string>
#include <map>

#include "azer/base/config_node.h"
#include "azer/files/res_path.h"
#include "azer/scene/scene_node.h"

namespace azer {
// create an scene template tree, which could be generate a scene tree
// xml
// format of scene config
// <xml>
// <node name="">
//   <children>
//     <node name="object1" type="node" path=":cylinder"/>
//     <node name="object2" type="subscene">
//      <position></position>
//     </node>
//   </children>
// </node>
// <node name="template">
//   <node name="box" type="mesh">
//     <position></position>
//     <mesh path=""/>
//   </node>
//   <node name="cylinder" type="mesh">
//     <position></position>
//     <mesh_path=""/>
//   </node>
// </node>
// </xml>

class SceneCreator {
 public:
  SceneCreator();
  
  SceneNodePtr Create(ConfigNodePtr root);
 protected:
  SceneNodePtr InitSceneNodeRecusive(ConfigNodePtr config_node);
  
  virtual SceneNodePtr CreateSceneNode(ConfigNodePtr config_node) = 0;
  bool InitSceneConfig(SceneNodePtr scene, ConfigNodePtr config);
  DISALLOW_COPY_AND_ASSIGN(SceneCreator);
};
}  // namespace azer
