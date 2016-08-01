#include "azer/util/geometry/cylinder.h"

#include "azer/render/index_pack.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/vertex_buffer.h"
#include "azer/util/geometry/common.h"
#include "azer/util/geometry/cone.h"
#include "azer/util/geometry/normal_util.h"

namespace azer {
int32_t CalcBarrelVertexCount(int32_t stack, int32_t slice) {
  return stack * (slice + 1);
  
}
int32_t CalcBarrelIndexCount(int32_t stack, int32_t slice) {
  return (stack - 1) * slice * 3 * 2;
}

Subset AppendGeoBarrelData(VertexPack* vpack, IndexPack* ipack, 
                           const GeoCylinderParam& p, const Matrix4& mat) {
  Subset subset;
  subset.vertex_base = vpack->index();
  VertexPos tpos;
  GetSemanticIndex("texcoord", 0, vpack->desc(), &tpos);
  float height_unit = p.height / ((float)p.stack - 1.0f);
  float radius_unit = (p.bottom_radius - p.top_radius) / ((float)p.stack - 1.0f);
  float slice_radius = p.top_radius;
  float y = p.height;
  // float tex_u_unit = 1.0f / p.slice;
  // float tex_v_unit = 1.0f / p.stack;

  for (int i = 0; i < p.stack; ++i) {
    for (int j = 0; j < p.slice + 1; ++j) {
      float degree = j * 360.0f / p.slice;
      float x = slice_radius * cos(Degree(degree));
      float z = slice_radius * sin(Degree(degree));

      Vector4 pos = std::move(mat * Vector4(x, y, z, 1.0f));
      vpack->WriteVector3Or4(pos, VertexPos(0, 0));
      UpdateVertexBounds(pos, &subset.vmin, &subset.vmax);
      // float u = j * tex_u_unit;
      // float v = i  * tex_v_unit;
      vpack->WriteVector2(Vector2(0.0f, 0.0f), tpos); 
      vpack->next(1);
    }
    slice_radius += radius_unit;
    y -= height_unit;
  }

  subset.vertex_count = vpack->index() - subset.vertex_base;
  subset.index_base = ipack->index();
  for (int i = 0; i < p.stack - 1; ++i) {
    int32_t line1 = i * (p.slice + 1); 
    int32_t line2 = (i + 1) * (p.slice + 1); 
    GenTriStripIndex(line1, line2, p.slice + 1, ipack);
  }
  subset.index_count = ipack->index() - subset.index_base;

  CalcIndexedTriangleNormal(vpack->data(), ipack->data(), subset);
  return subset;
}

void AppendGeoBarrelSubset(EntityData* data, const GeoCylinderParam& p, 
                             const Matrix4& mat) {
  VertexPack vpack(data->vdata());
  IndexPack ipack(data->idata());

  {
    const int32_t kVertexCount = CalcBarrelVertexCount(p.stack, p.slice);
    const int32_t kIndexCount = CalcBarrelIndexCount(p.stack, p.slice);
    data->vdata()->extend(kVertexCount);
    data->idata()->extend(kIndexCount);
    vpack.move(data->vdata()->vertex_count() - kVertexCount);
    ipack.move(data->idata()->count() - kIndexCount);
    Subset sub = AppendGeoBarrelData(&vpack, &ipack, p, mat);
    data->AddSubset(sub);
  }
}

void AppendGeoCylinderSubset(EntityData* data, const GeoCylinderParam& p, 
                             const Matrix4& mat) {
  VertexPack vpack(data->vdata());
  IndexPack ipack(data->idata());

  {
    const int32_t kVertexCount = CalcBarrelVertexCount(p.stack, p.slice);
    const int32_t kIndexCount = CalcBarrelIndexCount(p.stack, p.slice);
    data->vdata()->extend(kVertexCount);
    data->idata()->extend(kIndexCount);
    vpack.move(data->vdata()->vertex_count() - kVertexCount);
    ipack.move(data->idata()->count() - kIndexCount);
    Subset sub = AppendGeoBarrelData(&vpack, &ipack, p, mat);
    data->AddSubset(sub);
  }

  {
    const int32_t kVertexCount = CalcGeoRoundVertexCount(p.slice) * 2;
    const int32_t kIndexCount = CalcGeoRoundIndexCount(p.slice) * 2;
    data->vdata()->extend(kVertexCount);
    data->idata()->extend(kIndexCount);
    vpack.move(data->vdata()->vertex_count() - kVertexCount);
    ipack.move(data->idata()->count() - kIndexCount);

    Matrix4 mat1 = std::move(mat * Transform::RotateX(Degree(180.0)));
    Subset sub = AppendGeoRoundData(&vpack, &ipack, p.bottom_radius, p.slice, mat1);
    data->AddSubset(sub);

    Matrix4 mat2 = std::move(mat * Transform::Translate(0.0f, p.height, 0.0f));
    sub = AppendGeoRoundData(&vpack, &ipack, p.top_radius, p.slice, mat2);
    data->AddSubset(sub);
  }
}

EntityDataPtr CreateCylinder(VertexDesc* desc, const GeoCylinderParam& p,
                             const Matrix4& mat) {
  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata.get(), idata.get()));
  AppendGeoCylinderSubset(data.get(), p, mat);
  return data;
}

}  // namespace azer
