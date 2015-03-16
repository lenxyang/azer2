#pragma once

#include <string>

#include "azer/resources/repository_node.h"

namespace azer {
namespace test {
RepositoryNodePtr GenerateRepositoryTreeFromString(const std::string& str);
}  // namespace test
}  // namespace azer
