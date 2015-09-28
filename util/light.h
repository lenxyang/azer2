#pragma once

#include "azer/render/render.h"

struct DirLight{
  azer::Vector4 dir;
  azer::Vector4 diffuse;
  azer::Vector4 ambient;
};
