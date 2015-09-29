#pragma once

namespace azer {
class Arrow {
 public:
  Arrow();
  ~Arrow();

  void SetColor(const Vector4& color) { color_ = color;}
  void Update(const Camera& camera);
  void Render(Renderer* renderer);

  TransformHolder* GetTransformHolder() { return &holder_;}
  const TransformHolder* GetTransformHolder() const { return &holder_;}
 private:
  const DirLight* light_;
  Vector4 color_;
  Matrix4 world_;
  Matrix4 cone_world_;
  Matrix4 cylinder_world_;
  Matrix4 pvw_;
  TransformHolder holder_;

  GeometryObjectPtr cone_;
  GeometryObjectPtr cylinder_;
  ColoredDiffuseEffectPtr diffuse_effect_;
  DISALLOW_COPY_AND_ASSIGN(Arrow);
};

}  // namespace azer
