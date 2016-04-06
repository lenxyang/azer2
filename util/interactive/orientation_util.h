#pragma once

namespace azer {

class Vector3;
class Quaternion;

void CalcSceneOrientForZDirection(const Vector3& d, Quaternion* orient);

Vector3 RotateLightDir(const Vector3& d, const Quaternion& q);
}  // namespace azer
