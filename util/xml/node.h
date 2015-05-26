#pragma once

#include <map>
#include <string>
#include <vector>

#include "azer/util/xml/tree_node.h"

struct _xmlNode;

namespace util {
namespace xml {

class Element;
class Attribute;
class TextNode;
class OtherNode;
class CDataTextNode;
class XPath;
class XPathContext;
class XMLContext;

class Node: public TreeNode<Node> {
 public:
  enum Type {
    kUnknownNode = -1,
    kElementNode = 1,
    kAttributeNode,
    kTextNode,
    kCDataSectionNode,
    kEntityRefNode,
    kEntityNode,
    kPINode,
    kCommentNode,
    kDocumentNode,
    kDocumentTypeNode,
    kDocumentFragNode,
    kDocumentNotationNode,
    kHTMLDocuementNode,
    kDtdNode,
    kElementDeclNode,
    kAttributeDeclNode,
    kEntityDecl,
    kNamespaceDecl,
    kXIncludeStartNode,
    kXIncludeEndNode,
  };
  Node(Type type, _xmlNode* node);
  virtual ~Node();

  Type type() const { return type_;}

  void* extra() { return extra_;}
  void set_extra(void* v) { extra_ = v;}
  _xmlNode* raw() { return node_;}

  virtual Element* ToElement() {CHECK(false); return NULL;}  // NOLINT
  virtual Attribute* ToAttribute() {CHECK(false); return NULL;}  // NOLINT
  virtual TextNode* ToTextNode() {CHECK(false); return NULL;} // NOLINT
  virtual CDataTextNode* ToCDataTextNode() {CHECK(false); return NULL;} // NOLINT
  virtual OtherNode* ToOtherNode() { CHECK(false); return NULL;} // NOLINT

  virtual bool execute(const XPath& path, XPathContext* context) {
    CHECK(false);
    return false;
  }

  XMLContext* context() { return context_;}
 private:
  Type type_;
  _xmlNode* node_;
  void* extra_;
  XMLContext* context_;

  friend class XMLContext;
  DISALLOW_COPY_AND_ASSIGN(Node);
};
Node::Type GetNodeType(int t);
std::string NodeTypeName(Node::Type type);

class Element : public Node {
 public:
  Element(_xmlNode* node)
      : Node(kElementNode, node) {}
  const std::string& tag_name() const { return tag_name_;}
  void set_tag(const std::string& name) { tag_name_ = name;}
  const std::string& GetAttr(const std::string& name) const;
  virtual Element* ToElement() { return this;}

  virtual bool execute(const XPath& path, XPathContext* context);
  void AddAttr(const std::string& name, const std::string& value);

  /**
   *  从自身的 textnode 子节点获得文本
   * 只有当 Element 仅有一个子节点且子节点是 TextNode 时，才成功
   **/
  bool SetText(const std::string& text);
  bool GetText(std::string* text);
  std::string GetText();
 private:
  const std::string empty_;
  std::string tag_name_;
  std::map<std::string, std::string> attributes_;
  DISALLOW_COPY_AND_ASSIGN(Element);
};

class Attribute: public Node {
 public:
  Attribute(_xmlNode* node) : Node(kAttributeNode, node) {}

  virtual Attribute* ToAttribute() { return this;}

  const std::string& name() const { return name_;}
  const std::string& value() const { return value_;}

  void set_name(const std::string& name) { name_ = name;}
  void set_value(const std::string& value) { value_ = value;}
 private:
  std::string name_;
  std::string value_;
  DISALLOW_COPY_AND_ASSIGN(Attribute);
};

class TextNode: public Node {
 public:
  TextNode(_xmlNode* node): Node(kTextNode, node) {}
  virtual TextNode* ToTextNode() { return this;}
  const std::string& text() const { return text_;}
  void set_text(const std::string& tex) { text_ = tex;}
 private:
  std::string text_;
  DISALLOW_COPY_AND_ASSIGN(TextNode);
};

class CDataTextNode : public Node {
 public:
  CDataTextNode(_xmlNode* node): Node(kCDataSectionNode, node) {}
  virtual CDataTextNode* ToCDataTextNode() { return this;}
  const std::string& text() const { return text_;}
  void set_text(const std::string& tex) { text_ = tex;}
 private:
  std::string text_;
  DISALLOW_COPY_AND_ASSIGN(CDataTextNode);
};

class OtherNode: public Node {
 public:
  explicit OtherNode(Type t, _xmlNode* node)
      : Node(t, node) {
  }
  virtual OtherNode* ToOtherNode() { return this;}
 private:
  DISALLOW_COPY_AND_ASSIGN(OtherNode);
};
}  // namespace xml
}  // namespace util
