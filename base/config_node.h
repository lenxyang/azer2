#pragma once

#include <vector>
#include <map>

#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"

#include "azer/base/resource.h"
#include "azer/files/res_path.h"
#include "azer/math/math.h"
#include "azer/util/xml/xml.h"

namespace azer {
class ConfigNode;
typedef scoped_refptr<ConfigNode> ConfigNodePtr;
typedef std::vector<ConfigNodePtr> ConfigNodes;
class AZER_EXPORT ConfigNode : public Resource {
 public:
  typedef std::pair<std::string, std::string> Attribute;
  typedef std::vector<Attribute> Attributes;

  static ConfigNodePtr InitFromXML(util::xml::Element* element);
  static ConfigNodePtr InitFromXMLStr(const std::string& str);

  ConfigNode();

  const std::string& tagname() const { return tagname_;}
  ConfigNode* parent() { return parent_;}
  const ConfigNode* parent() const { return parent_;}
  ConfigNode* root();
  const ConfigNode* root() const;

  void AddAttr(const std::string& name, const std::string& value);
  const Attributes& attributes() const { return attrs_;}
  const std::string GetAttr(const std::string& name) const;
  bool HasAttr(const std::string& name) const;
  bool GetAttrAsDouble(const std::string& name, double* v) const;
  bool GetAttrAsFloat(const std::string& name, float* v) const;
  bool GetAttrAsInt(const std::string& name, int32* v) const;
  bool GetAttrAsVec2(const std::string& name, Vector2* v) const;
  bool GetAttrAsVec3(const std::string& name, Vector3* v) const;
  bool GetAttrAsVec4(const std::string& name, Vector4* v) const;
  bool GetAttrAsQuaternion(const std::string& name, Quaternion* v) const;

  const std::string& GetText() const;
  bool GetTextAsDouble(double* v) const;
  bool GetTextAsFloat(float* v) const;
  bool GetTextAsInt(int32* v) const;
  bool GetTextAsVec2(Vector2* v) const;
  bool GetTextAsVec3(Vector3* v) const;
  bool GetTextAsVec4(Vector4* v) const;
  bool GetTextAsQuaternion(Quaternion* v) const;

  std::string GetChildTextString(const std::string& name) const;
  bool GetChildText(const std::string& name, std::string* text) const;
  bool GetChildTextAsDouble(const std::string& name, double* v) const;
  bool GetChildTextAsFloat(const std::string& name, float* v) const;
  bool GetChildTextAsInt(const std::string& name, int32* v) const;
  bool GetChildTextAsVec2(const std::string& name, Vector2* v) const;
  bool GetChildTextAsVec3(const std::string& name, Vector3* v) const;
  bool GetChildTextAsVec4(const std::string& name, Vector4* v) const;
  bool GetChildTextAsQuaternion(const std::string& name, Quaternion* v) const;

  const ConfigNodes& children() const { return children_;}
  ConfigNodes& children() { return children_;}
  int32 child_count() const { return static_cast<int>(children_.size());}
  const ConfigNodePtr& child_at(int32 index) const;
  ConfigNodePtr& child_at(int32 index);

  bool AddChild(ConfigNodePtr node);
  bool Contains(ConfigNode* node) const;
  bool HasNamedChild(const std::string& name) const;
  std::vector<ConfigNodePtr> GetNamedChildren(const std::string& name) const;
  ConfigNodePtr GetFirstChildNamed(const std::string& name) const;
  ConfigNodePtr GetLastChildNamed(const std::string& name) const;

  std::string print_info();
 private:
  std::string print_node_info(int depth);
  std::string print_node_recusive(int depth);
  bool InitFromXMLNodeRecusive(util::xml::Element* element);
  bool InitFromXMLNode(util::xml::Element* element);
  ConfigNodes children_;
  ConfigNode* parent_;
  Attributes attrs_;
  std::string tagname_;
  std::string text_;
  DISALLOW_COPY_AND_ASSIGN(ConfigNode);
};
}  // namespace azer
