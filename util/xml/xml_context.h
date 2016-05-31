#pragma once

#include <string>
#include <vector>

#include "azer/util/xml/node.h"

namespace util {
namespace xml {

class XPath;
class XPathContext;

class XMLContext {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}
    virtual void OnElementBegin() = 0;
    virtual void OnElementEnd() = 0;
    virtual void OnAttributes() = 0;
  };

  XMLContext();
  ~XMLContext();

  void traverse(Delegate* delegate);
  bool execute(const XPath& path, XPathContext* context);
  Node* root() { return root_;}
  void Reset();
 private:
  bool BuildTree();
  void BuildTreeRecusive(void* parent, Node* node);

  Node* CreateNode(int type, void *ptr);
  void InitNode(Node* node, void* ptr);
  void InitElement(Element* node, void* ptr);
  friend class XPath;
  friend class XPathContext;
  friend class XMLParser;
  void* doc_;
  Node* root_;
  std::vector<Node*> allocated_;
  DISALLOW_COPY_AND_ASSIGN(XMLContext);
};

class TreeDumper : public TreeNode<Node>::Traverser {
 public:
  TreeDumper(): depth_(0) {}
  void Dump(Node* node);
 protected:
  virtual bool OnVisitBegin(Node* node);
  virtual void OnVisitEnd(Node* node);
 private:
  int depth_;
  DISALLOW_COPY_AND_ASSIGN(TreeDumper);
};
}  // namespace xml
}  // namespace util
