#pragma once

#include <string>
#include <vector>
#include "base/basictypes.h"

// libxml
struct _xmlDoc;

namespace util {
namespace xml {
class Node;
class XMLContext;
class Element;

class XPath {
 public:
  explicit XPath(const std::string& path);
  XPath(const XPath& xpath);
  ~XPath();

  XPath& operator = (const XPath& xpath);

  const std::string& expression() const { return path_express_;}
 private:
  friend class XMLContext;
  std::string path_express_;
};

class XPathContext {
 public:
  XPathContext();
  ~XPathContext();

  const std::vector<Node*> GetResult() const { return vec_;}
  void Reset();
 private:
  bool Init(const XPath& xpath, XMLContext* context);
  bool Init(const XPath& xpath, Element* context);
  bool Init(const XPath& xpath, _xmlDoc* doc);
  friend class XMLContext;
  friend class XPath;
  friend class Element;
  void* xpath_ctx_;
  void* xpath_obj_;
  std::vector<Node*> vec_;
  DISALLOW_COPY_AND_ASSIGN(XPathContext);
};

}  // namespace xml
}  // namespace util
