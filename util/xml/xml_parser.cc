#include "azer/util/xml/xml_parser.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "base/logging.h"

namespace util {
namespace xml {
bool XMLParser::Parse(const std::string& text, XMLContext* context) {
  DCHECK(context->doc_ == NULL);
  context->doc_ = xmlReadMemory(text.c_str(), text.length(), "", NULL, 0);
  if (context->doc_ == NULL) {
    return false;
  }

  context->BuildTree();
  return true;
}

}  // namespace xml
}  // namespace util
