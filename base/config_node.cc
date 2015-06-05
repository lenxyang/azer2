#include "azer/base/config_node.h"

#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_split.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"

#include "azer/base/string.h"


namespace azer {

namespace {
bool StringToDoubleVec(const std::string& text, std::vector<double>* vec) {
  std::vector<std::string> v;
  double dv;
  ::base::SplitString(text, ',', &v);
  for (auto iter = v.begin(); iter != v.end(); ++iter) {
    if (!::base::StringToDouble(*iter, &dv))
      return false;

    vec->push_back(dv);
  }
  return true;
}
}  // namespace

ConfigNode::ConfigNode()
    : Resource(kConfig),
      parent_(NULL) {
}

ConfigNode::ConfigNode(const ResPath& path)
    : Resource(kConfig),
      parent_(NULL),
      respath_(path) {
}

const ResPath& ConfigNode::respath() const {
  return root()->respath_;
}

ResPath ConfigNode::package_path() const  {
  return ResPath(std::move(respath().filepath()));
}

ConfigNode* ConfigNode::root() {
  return const_cast<ConfigNode*>(const_cast<const ConfigNode*>(this)->root());
}

const ConfigNode* ConfigNode::root() const {
  const ConfigNode* node = this;
  while (node) {
    if (node->parent_) {
      node = node->parent_;
    } else {
      return node;
    }
  }

  NOTREACHED();
  return NULL;
}

const ConfigNodePtr& ConfigNode::child_at(int32 index) const {
  DCHECK_LT(index, child_count());
  return children_[index];
}

ConfigNodePtr& ConfigNode::child_at(int32 index) {
  DCHECK_LT(index, child_count());
  return children_[index];
}

bool ConfigNode::AddChild(ConfigNodePtr node) {
  DCHECK(!node->parent());
  if (Contains(node)) {
    return false;
  }

  node->parent_ = this;
  children_.push_back(node);
  return true;
}

bool ConfigNode::Contains(ConfigNode* node) const {
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if (iter->get() == node)
      return true;
  }

  return false;
}

std::vector<ConfigNodePtr> ConfigNode::GetNamedChildren(
    const std::string& name) const {
  std::vector<ConfigNodePtr> vec;
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if ((*iter)->tagname() == name)
      vec.push_back(*iter);
  }

  return vec;
}

ConfigNodePtr ConfigNode::GetFirstChildNamed(
    const std::string& name) const {
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if ((*iter)->tagname() == name)
      return *iter;
  }

  return ConfigNodePtr();
}

ConfigNodePtr ConfigNode::GetLastChildNamed(
    const std::string& name) const {
  for (auto iter = children_.rbegin(); iter != children_.rend(); ++iter) {
    if ((*iter)->tagname() == name)
      return *iter;
  }

  return ConfigNodePtr();
}

bool ConfigNode::HasNamedChild(const std::string& name) const {
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if ((*iter)->tagname() == name)
      return true;
  }

  return false;
}

bool ConfigNode::HasAttr(const std::string& name) const {
  for (auto iter = attrs_.begin(); iter != attrs_.end(); ++iter) {
    if (iter->first == name)
      return true;
  }
  return false;
}

const std::string ConfigNode::GetAttr(const std::string& name) const {
  for (auto iter = attrs_.begin(); iter != attrs_.end(); ++iter) {
    if (iter->first == name)
      return iter->second;
  }
  return std::string("");
}

bool ConfigNode::GetAttrAsDouble(const std::string& name, double* v) const {
  std::string str = std::move(GetAttr(name));
  return ::base::StringToDouble(str, v);
}

bool ConfigNode::GetAttrAsFloat(const std::string& name, float* v) const {
  double dv;
  bool ret = GetAttrAsDouble(name, &dv);
  if (ret) *v = (float)dv;
  return ret;
}

bool ConfigNode::GetAttrAsInt(const std::string& name, int32* v) const {
  std::string str = std::move(GetAttr(name));
  return ::base::StringToInt(str, v);
}

const std::string& ConfigNode::GetText() const {
  return text_;
}

bool ConfigNode::GetTextAsDouble(double* v) const {
  return ::base::StringToDouble(text_, v);
}

bool ConfigNode::GetTextAsFloat(float* v) const {
  double dv;
  bool ret = GetTextAsDouble(&dv);
  if (ret) *v = (float)dv;
  return ret;
}

bool ConfigNode::GetTextAsInt(int32* v) const {
  return ::base::StringToInt(text_, v);
}

bool ConfigNode::GetTextAsVec2(Vector2* v) const {
  std::vector<double> vec;
  bool ret = StringToDoubleVec(text_, &vec);
  if (!ret || vec.size() != 2u)
    return false;
  v->x = (float)vec[0];
  v->y = (float)vec[1];
  return true;
}

bool ConfigNode::GetTextAsVec3(Vector3* v) const {
  std::vector<double> vec;
  bool ret = StringToDoubleVec(text_, &vec);
  if (!ret || vec.size() != 3u)
    return false;
  v->x = (float)vec[0];
  v->y = (float)vec[1];
  v->z = (float)vec[2];
  return true;
}

bool ConfigNode::GetTextAsVec4(Vector4* v) const {
  std::vector<double> vec;
  bool ret = StringToDoubleVec(text_, &vec);
  if (!ret || vec.size() != 4u)
    return false;
  v->x = (float)vec[0];
  v->y = (float)vec[1];
  v->z = (float)vec[2];
  v->w = (float)vec[3];
  return true;
}

bool ConfigNode::GetTextAsQuaternion(Quaternion* v) const {
  std::vector<double> vec;
  bool ret = StringToDoubleVec(text_, &vec);
  if (!ret || vec.size() != 4u)
    return false;
  *v = Quaternion((float)vec[0], (float)vec[1], (float)vec[2], (float)vec[3]);
  return true;
}

bool ConfigNode::GetChildText(const std::string& name, std::string* text) const {
  std::vector<ConfigNodePtr> vec = std::move(GetNamedChildren(name));
  if (vec.size() != 1u)
    return false;
  *text = vec[0]->GetText();
  return true;
}

bool ConfigNode::GetChildTextAsDouble(const std::string& name, double* v) const {
  std::vector<ConfigNodePtr> vec = std::move(GetNamedChildren(name));
  if (vec.size() != 1u)
    return false;
  return vec[0]->GetTextAsDouble(v);
}

bool ConfigNode::GetChildTextAsFloat(const std::string& name, float* v) const {
  std::vector<ConfigNodePtr> vec = std::move(GetNamedChildren(name));
  if (vec.size() != 1u)
    return false;
  return vec[0]->GetTextAsFloat(v);
}

bool ConfigNode::GetChildTextAsInt(const std::string& name, int32* v) const {
  std::vector<ConfigNodePtr> vec = std::move(GetNamedChildren(name));
  if (vec.size() != 1u)
    return false;
  return vec[0]->GetTextAsInt(v);
}

bool ConfigNode::GetChildTextAsVec2(const std::string& name, Vector2* v) const {
  std::vector<ConfigNodePtr> vec = std::move(GetNamedChildren(name));
  if (vec.size() != 1u)
    return false;
  return vec[0]->GetTextAsVec2(v);
}

bool ConfigNode::GetChildTextAsVec3(const std::string& name, Vector3* v) const {
  std::vector<ConfigNodePtr> vec = std::move(GetNamedChildren(name));
  if (vec.size() != 1u)
    return false;
  return vec[0]->GetTextAsVec3(v);
}

bool ConfigNode::GetChildTextAsVec4(const std::string& name, Vector4* v) const {
  std::vector<ConfigNodePtr> vec = std::move(GetNamedChildren(name));
  if (vec.size() != 1u)
    return false;
  return vec[0]->GetTextAsVec4(v);
}

bool ConfigNode::GetChildTextAsQuaternion(const std::string& name, Quaternion* v) const {
  std::vector<ConfigNodePtr> vec = std::move(GetNamedChildren(name));
  if (vec.size() != 1u)
    return false;
  return vec[0]->GetTextAsQuaternion(v);
}



void ConfigNode::AddAttr(const std::string& name, const std::string& value) {
  attrs_.push_back(std::make_pair(name, value));
}

bool ConfigNode::InitFromXML(util::xml::Element* ele) {
  return InitFromXMLNodeRecusive(ele);
}

bool ConfigNode::InitFromXMLNode(util::xml::Element* element) {
  tagname_ = element->tag_name();
  text_ = element->GetText();
  for (auto iter = element->attributes().begin();
       iter != element->attributes().end();
       ++iter) {
    attrs_.push_back(std::make_pair(iter->first, iter->second));
  }
 
  return true;
}

bool ConfigNode::InitFromXMLNodeRecusive(util::xml::Element* element) {
  if (!InitFromXMLNode(element)) {
    return false;
  }

  util::xml::Node* child = element->first_child();
  for (; child; child = child->next_sibling()) {
    if (child->type() == util::xml::Node::kElementNode) {
      util::xml::Element* ele = child->ToElement();
      ConfigNodePtr ptr = new ConfigNode();
      if (!ptr->InitFromXMLNodeRecusive(ele) || !this->AddChild(ptr)) {
        return false;
      }
    }
  }

  return true;
}

std::string ConfigNode::print_info() {
  return print_node_recusive(0);
}

std::string ConfigNode::print_node_info(int depth) {
  std::string str;
  str.append(std::string(depth, ' '));
  str.append(std::move(::base::StringPrintf("[tag=%s]", tagname_.c_str())));
  if (attrs_.size() > 0) {
    str.append("{");
    for (auto iter = attrs_.begin(); iter != attrs_.end(); ++iter) {
      if (iter != attrs_.begin())
        str.append(" ");
      str.append(
          ::base::StringPrintf("%s:%s", iter->first.c_str(), iter->second.c_str()));
    }
    str.append("}");
  }
  str.append("\n");
  return str;
}

std::string ConfigNode::print_node_recusive(int depth) {
  std::string str = std::move(print_node_info(depth));
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    str.append(std::move((*iter)->print_node_recusive(depth + 1)));
  }

  return str;
}
}  // namespace azer
