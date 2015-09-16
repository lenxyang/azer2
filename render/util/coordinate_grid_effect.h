#pragma once

#include "azer/math/math.h"
#include "azer/render/effect.h"
#include "azer/render/technique.h"
#include "azer/render/vertex_buffer.h"

namespace azer {

class Renderer;
class RenderSystem;

class AZER_EXPORT CoordinateGridEffect : public Effect {
 public:
  static const char kEffectName[];
  CoordinateGridEffect(RenderSystem* rs);

#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 vp;
  };
  struct ps_cbuffer {
    Vector4 diffuse;
  };
#pragma pack(pop)
  const char* name() const override;
  struct Vertex {
    Vector4 position;
    Vertex(const Vector4& in_position)
        : position(in_position)
    {}
    Vertex() {}
  };

  void SetPVW(const Matrix4& value);
  void SetGridDiffuse(const Vector4& value);

  VertexDescPtr GetVertexDesc() { return technique_->GetVertexDesc();}
  static const int kVertexDescNum;
  static const VertexDesc::Desc kVertexDesc[];
 protected:
  void InitShader();
  bool Init(const ShaderPrograms& prog) override { return true;}
  void Init(RenderSystem* rs);

  virtual void UseTexture(Renderer* renderer) override;
  DISALLOW_COPY_AND_ASSIGN(CoordinateGridEffect);
};
}  // namespace azer
