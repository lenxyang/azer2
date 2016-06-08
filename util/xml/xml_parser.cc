#include "azer/util/xml/xml_parser.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "base/logging.h"

namespace util {
namespace xml {
bool XMLParser::Parse(const std::string& text, XMLContext* context) {
  DCHECK(context->doc_ == NULL);
  int32_t len = static_cast<int32_t>(text.length());
  context->doc_ = xmlReadMemory(text.c_str(), len, "", NULL, 0);
  if (context->doc_ == NULL) {
    return false;
  }

  context->BuildTree();
  return true;
}

}  // namespace xml
}  // namespace util
