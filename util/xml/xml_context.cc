#include "azer/util/xml/xml_context.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#include <iostream>

#include "base/logging.h"
#include "azer/util/xml/xpath.h"

namespace util {
namespace xml {

XMLContext::XMLContext()
    : doc_(NULL)
    , root_(NULL) {
}

void XMLContext::Reset() {
  for (auto iter = allocated_.begin(); iter != allocated_.end(); ++iter) {
    delete *iter;
  }
  allocated_.clear();
  if (doc_) {
    xmlFreeDoc((xmlDocPtr)doc_);
    doc_ = NULL;
  }
}

XMLContext::~XMLContext() {
  Reset();
}

void XMLContext::traverse(Delegate* delegate) {
  // root_->traverse(delegate);
}

bool XMLContext::execute(const XPath& path, XPathContext* context) {
  DCHECK(context != NULL);
  if (!context->Init(path, this)) return false;
  return true;
}

void XMLContext::BuildTreeRecusive(void* n, Node* node) {
  xmlNode *parent = (xmlNode*)n;  // NOLINT
  xmlNode* cur_node = parent->children;
  for (; cur_node; cur_node = cur_node->next) {
    Node* newnode = CreateNode(cur_node->type, cur_node);
    node->AddChild(newnode);
    BuildTreeRecusive(cur_node, newnode);
  }
}

bool XMLContext::BuildTree() {
  xmlNode* root = xmlDocGetRootElement((xmlDoc*)doc_);  // NOLINT
  if (!root) { return false;}
  root_ = CreateNode(root->type, root);
  BuildTreeRecusive(root, root_);
  return true;
}

void XMLContext::InitElement(Element* node, void* ptr) {
  xmlNode* xmlnode =  (xmlNode*)ptr;  // NOLINT
  xmlAttrPtr attr = xmlnode->properties;
  for (; NULL != attr; attr = attr->next) {
    if (attr->name) {
      xmlChar* value = xmlNodeListGetString(xmlnode->doc, attr->children, 1);
      node->AddAttr((const char*)attr->name, (const char*)value);
      xmlFree(value);
    }
  }
}

Node* XMLContext::CreateNode(int type, void* extra) {
  xmlNode* xmlnode =  (xmlNode*)extra;  // NOLINT
  DCHECK(extra != NULL);
  Node* node = NULL;
  switch (type) {
    case XML_ELEMENT_NODE:
      node = new Element(xmlnode);
      node->ToElement()->set_tag((const char*)xmlnode->name);
      InitElement(node->ToElement(), extra);
      break;
    case XML_ATTRIBUTE_NODE:
      node = new Attribute(xmlnode);
      node->ToAttribute()->set_name((const char*)xmlnode->name);
      break;
    case XML_CDATA_SECTION_NODE: {
      node = new CDataTextNode(xmlnode);
      xmlChar *key = xmlNodeGetContent(xmlnode);
      node->ToCDataTextNode()->set_text((const char*)key);
      xmlFree(key);
    }
      break;
    case XML_TEXT_NODE: {
      node = new TextNode(xmlnode);
      xmlChar *key = xmlNodeGetContent(xmlnode);
      node->ToTextNode()->set_text((const char*)key);
      xmlFree(key);
    }
      break;
    default:
      node = new OtherNode(GetNodeType(type), xmlnode);
      break;
  }

  xmlnode->_private = node;  // NOLINT
  node->set_extra(extra);
  allocated_.push_back(node);
  return node;
}

void TreeDumper::Dump(Node* node) {
  node->traverse(this);
}

bool TreeDumper::OnVisitBegin(Node* node) {
  std::string ident(depth_, ' ');
  std::cout << ident << NodeTypeName(node->type());
  if (node->type() == Node::kElementNode) {
    std::cout << "{" << node->ToElement()->tag_name() << "}";
  } else if (node->type() == Node::kTextNode) {
    std::cout << "{" << node->ToTextNode()->text() << "}";
  } else if (node->type() == Node::kAttributeNode) {
    std::cout << "{" << node->ToAttribute()->name() << "}";
  } else if (node->type() == Node::kCDataSectionNode) {
    std::cout << "{" << node->ToCDataTextNode()->text() << "}";
  }
  std::cout << std::endl;
  depth_++;
  return true;
}

void TreeDumper::OnVisitEnd(Node* node) {
  depth_--;
}
}  // namespace xml
}  // namespace util
