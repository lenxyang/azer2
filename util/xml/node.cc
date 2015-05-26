#include "azer/util/xml/node.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <string>

#include "azer/util/xml/xpath.h"
#include "azer/util/xml/tree_node.h"

namespace util {
namespace xml {

Node::Node(Type type, _xmlNode* node)
    : type_(type),
      node_(node),
      extra_(NULL),
      context_(NULL){
}

Node::~Node() {
}

const std::string& Element::GetAttr(const std::string& name) const {
  auto iter = attributes_.find(name);
  if (iter != attributes_.end()) {
    return iter->second;
  } else {
    return empty_;
  }
}
void Element::AddAttr(const std::string& name, const std::string& value) {
  attributes_.insert(std::make_pair(name, value));
}

bool Element::GetText(std::string* text) {
  text->clear();
  if (first_child() == NULL) {
    return false;
  } else {
    if (first_child()->type() == kTextNode) {
      *text = first_child()->ToTextNode()->text();
    } else if (first_child()->type() == kCDataSectionNode) {
      *text = first_child()->ToCDataTextNode()->text();
    } else {
      return false;
    }
  }

  return true;
}

bool Element::SetText(const std::string& text) {
  if (first_child() == NULL) {
    return false;
  } else {
    if (first_child()->type() == kTextNode) {
      first_child()->ToTextNode()->set_text(text);
    } else if (first_child()->type() == kCDataSectionNode) {
      first_child()->ToCDataTextNode()->set_text(text);
    } else {
      return false;
    }
  }
  return true;
}

std::string Element::GetText() {
  std::string str;
  GetText(&str);
  return str;
}

bool Element::execute(const XPath& path, XPathContext* context) {
  DCHECK(context != NULL);
  if (!context->Init(path, this)) return false;
  return true;
}


Node::Type GetNodeType(int t) {
  switch (t) {
    case XML_ELEMENT_NODE: return Node::kElementNode;
    case XML_ATTRIBUTE_NODE: return Node::kAttributeNode;
    case XML_TEXT_NODE: return Node::kTextNode;
    case XML_CDATA_SECTION_NODE: return Node::kCDataSectionNode;
    case XML_ENTITY_REF_NODE: return Node::kEntityRefNode;
    case XML_ENTITY_NODE: return Node::kEntityNode;
    case XML_PI_NODE: return Node::kPINode;
    case XML_COMMENT_NODE: return Node::kCommentNode;
    case XML_DOCUMENT_NODE: return Node::kDocumentNode;
    case XML_DOCUMENT_TYPE_NODE: return Node::kDocumentTypeNode;
    case XML_DOCUMENT_FRAG_NODE: return Node::kDocumentFragNode;
    case XML_NOTATION_NODE: return Node::kDocumentNotationNode;
    case XML_HTML_DOCUMENT_NODE: return Node::kHTMLDocuementNode;
    case XML_DTD_NODE: return Node::kDtdNode;
    case XML_ELEMENT_DECL: return Node::kElementDeclNode;
    case XML_ATTRIBUTE_DECL: return Node::kAttributeDeclNode;
    case XML_ENTITY_DECL: return Node::kEntityDecl;
    case XML_NAMESPACE_DECL: return Node::kNamespaceDecl;
    case XML_XINCLUDE_START: return Node::kXIncludeStartNode;
    case XML_XINCLUDE_END: return Node::kXIncludeEndNode;
    default: CHECK(false) << "Not such type: " << t;
      return Node::kUnknownNode;
  }
}

std::string NodeTypeName(Node::Type type) {
  switch (type) {
    case Node::kElementNode: return "ElementNode";
    case Node::kAttributeNode: return "AttributeNode";
    case Node::kTextNode: return "TextNode";
    case Node::kCDataSectionNode: return "CDataSectionNode";
    case Node::kEntityRefNode: return "EntityRefNode";
    case Node::kEntityNode: return "EntityNode";
    case Node::kPINode: return "PINode";
    case Node::kCommentNode: return "CommentNode";
    case Node::kDocumentNode: return "DocumentNode";
    case Node::kDocumentTypeNode: return "DocumentTypeNode";
    case Node::kDocumentFragNode: return "DocumentFragNode";
    case Node::kDocumentNotationNode: return "DocumentNotationNode";
    case Node::kHTMLDocuementNode: return "HTMLDocuementNode";
    case Node::kDtdNode: return "DtdNode";
    case Node::kElementDeclNode: return "ElementDeclNode";
    case Node::kAttributeDeclNode: return "AttributeDeclNode";
    case Node::kEntityDecl: return "EntityDecl";
    case Node::kNamespaceDecl: return "NamespaceDecl";
    case Node::kXIncludeStartNode: return "XIncludeStartNode";
    case Node::kXIncludeEndNode: return "XIncludeEndNode";
    case Node::kUnknownNode: return "unknown";
    default: return "Unkonwn Node";
  }
}

}  // namespace xml
}  // namespace util
