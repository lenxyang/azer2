#include "azer/util/xml/xpath.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#include "base/logging.h"
#include "azer/util/xml/xml_context.h"
#include "azer/util/xml/node.h"

namespace util {
namespace xml {

XPathContext::XPathContext()
    : xpath_ctx_(NULL)
    , xpath_obj_(NULL) {
}

XPathContext::~XPathContext() {
  Reset();
}

void XPathContext::Reset() {
  if (xpath_obj_) {
    xmlXPathFreeObject((xmlXPathObjectPtr)xpath_obj_);
    xpath_obj_ = NULL;
  }

  if (xpath_ctx_) {
    xmlXPathFreeContext((xmlXPathContextPtr)xpath_ctx_);
    xpath_ctx_ = NULL;
  }
}

bool XPathContext::Init(const XPath& xpath, Element* element) {
  return Init(xpath, (xmlDocPtr)(element->raw()));
}

bool XPathContext::Init(const XPath& xpath, XMLContext* context) {
  return Init(xpath, (xmlDocPtr)context->doc_);
}

bool XPathContext::Init(const XPath& xpath, xmlDoc* doc) {
  DCHECK(xpath_ctx_ == NULL && xpath_obj_ == NULL) << "XPath has been initialize";
  xpath_ctx_ = (xmlXPathContextPtr)xmlXPathNewContext(doc);
  if (!xpath_ctx_) {
    LOG(ERROR) << "Failed to init XMLContext.";
    return false;
  }

  xpath_obj_ = (xmlXPathObjectPtr)xmlXPathEvalExpression(
      (const xmlChar*)xpath.expression().c_str(),
      (xmlXPathContextPtr)xpath_ctx_);
  if (!xpath_obj_) {
    LOG(ERROR) << "Failed to init XMLContext.";
    return false;
  }

  xmlNodeSetPtr nodes = ((xmlXPathObjectPtr)xpath_obj_)->nodesetval;
  const int num = nodes ? nodes->nodeNr: 0;
  for (int i = 0; i < num; ++i) {
    vec_.push_back((Node*)(nodes->nodeTab[i]->_private));  // NOLINT
  }

  return true;
}

XPath::XPath(const std::string& path)
    : path_express_(path) {
}
XPath::XPath(const XPath& path)
    : path_express_(path.expression()) {
}


XPath::~XPath() {
}

XPath& XPath::operator = (const XPath& xpath) {
  path_express_ = xpath.expression();
  return *this;
}
}  // namespace xml
}  // namespace util
