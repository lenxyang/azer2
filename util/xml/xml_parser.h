#pragma once

#include <string>
#include "base/basictypes.h"
#include "azer/util/xml/xml_context.h"

namespace util {
namespace xml {
class XMLParser {
 public:
  XMLParser() {}

  bool Parse(const std::string& text, XMLContext* context);
 private:
  DISALLOW_COPY_AND_ASSIGN(XMLParser);
};
}  // namespace xml
}  // namespace util
