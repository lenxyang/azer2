#pragma once

#include <string>

#include "azer/resources/repository_node.h"

namespace azer {
namespace resources {
namespace test {
RepositoryNodePtr GenerateRepositoryTreeFromString(const std::string& str);
}  // namespace test
}  // namespace resources
}  // namespace azer
