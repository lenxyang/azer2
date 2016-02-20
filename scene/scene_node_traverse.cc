#include "azer/scene/scene_node_traverse.h"

#include "azer/scene/scene_node.h"

namespace azer {
SceneNodeTraverse::SceneNodeTraverse(SceneNodeTraverseDelegate* delegate) 
    : delegate_(delegate) {
}

SceneNodeTraverse::~SceneNodeTraverse() {
}

void SceneNodeTraverse::Traverse(SceneNode* root) {
  delegate_->OnTraverseBegin(root);
  TraverseNode(root);
  delegate_->OnTraverseEnd();
}

void SceneNodeTraverse::TraverseNode(SceneNode* node) {
  if (delegate_->OnTraverseNodeEnter(node)) {
    for (int32 i = 0; i < node->child_count(); ++i) {
      SceneNode* child = node->child_at(i);
      TraverseNode(child);
    }
  }
  delegate_->OnTraverseNodeExit(node);
}
}  // namespace azer
