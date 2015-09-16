#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/effect.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/technique.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
class AZER_EXPORT CoordinateGridEffect : public azer::Effect {
 public:
  static const char kEffectName[];
  CoordinateGridEffect(azer::RenderSystem* rs);

  bool Init(const ShaderPrograms& prog) override {return true;}
#pragma pack(push, 4)
  struct vs_cbuffer {
    azer::Matrix4 vp;
  };
  struct ps_cbuffer {
    azer::Vector4 diffuse;
  };
#pragma pack(pop)
  const char* name() const override;
  struct Vertex {
    azer::Vector4 position;
    Vertex(const azer::Vector4& in_position)
        : position(in_position)
    {}
    Vertex() {}
  };

  void SetPVW(const azer::Matrix4& value);
  void SetGridDiffuse(const azer::Vector4& value);

  azer::VertexDescPtr GetVertexDesc() { return technique_->GetVertexDesc();}
  static const int kVertexDescNum;
  static const azer::VertexDesc::Desc kVertexDesc[];
 protected:
  void Init(RenderSystem* rs);
  virtual void UseTexture(azer::Renderer* renderer) override;
  DISALLOW_COPY_AND_ASSIGN(CoordinateGridEffect);
};

class AZER_EXPORT CoordinateGrid {
 public:
  CoordinateGrid(int width, int height, int num);
  ~CoordinateGrid();

  void Render(Renderer* rs);
  void SetXCoordColor(const Vector4& col) { x_color_ = col;}
  void SetZCoordColor(const Vector4& col) { z_color_ = col;}
  void SetPVWMat(const Matrix4& mat);
 private:
  void Init();

  scoped_refptr<CoordinateGridEffect> effect_ptr_;
  VertexDataPtr data_;
  VertexBufferPtr vb_;
  Vector4 x_color_, z_color_;
  const int kWidth, kHeight, kNum;
  DISALLOW_COPY_AND_ASSIGN(CoordinateGrid);
};
}  // namespace azer
