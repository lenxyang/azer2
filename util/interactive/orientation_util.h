#pragma once

namespace azer {

class Vector3;
class Quaternion;

void CalcSceneOrientForZDirection(const Vector3& d, Quaternion* orient);
}  // namespace azer
