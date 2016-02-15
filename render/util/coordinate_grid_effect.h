#pragma once

#include "azer/math/math.h"
#include "azer/render/effect.h"
#include "azer/render/technique.h"
#include "azer/render/vertex_buffer.h"

namespace azer {

class Renderer;
class RenderSystem;

class CoordinateGridEffect : public Effect {
 public:
  static const char kEffectName[];
  CoordinateGridEffect();

#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 vp;
  };
  struct ps_cbuffer {
    Vector4 diffuse;
  };
#pragma pack(pop)
  const char* GetEffectName() const override;
  struct Vertex {
    Vector4 position;
    Vertex(const Vector4& in_position)
        : position(in_position)
    {}
    Vertex() {}
  };

  void SetPVW(const Matrix4& value);
  void SetGridDiffuse(const Vector4& value);
  static const int kVertexDescNum;
  static const VertexDesc::Desc kVertexDesc[];
 protected:
  void InitEffect();
  bool Init(VertexDesc* desc, const TechSource& prog) override {return true;}
  void ApplyGpuConstantTable(Renderer* renderer) override;

  vs_cbuffer vs_data_;
  ps_cbuffer ps_data_;
  DISALLOW_COPY_AND_ASSIGN(CoordinateGridEffect);
};
}  // namespace azer
