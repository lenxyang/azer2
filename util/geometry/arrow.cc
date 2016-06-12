#include "azer/util/geometry/arrow.h"

#include "azer/render/index_pack.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/vertex_buffer.h"
#include "azer/util/geometry/common.h"
#include "azer/util/geometry/normal_util.h"
#include "azer/util/geometry/cone.h"
#include "azer/util/geometry/cylinder.h"

namespace azer {

EntityDataPtr CreateArrow(VertexDesc* desc) {
  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata.get(), idata.get()));
  Matrix4 rot = std::move(RotateX(Degree(-90.0f)));

  GeoConeParam cone_param;
  cone_param.height = 0.25f;
  cone_param.radius = 0.125f;
  cone_param.slice = 64;
  Matrix4 cone_mat = rot * Translate(0.0f, 0.75f, 0.0f);
  AppendGeoConeData(data.get(), cone_param, cone_mat);

  GeoCylinderParam cylinder_param;
  cylinder_param.top_radius = 0.05f;
  cylinder_param.bottom_radius = cylinder_param.top_radius;
  cylinder_param.height = 0.75f;
  cylinder_param.slice = 64;
  cylinder_param.stack = 5;
  AppendGeoCylinderSubset(data.get(), cylinder_param, rot);
  return data;
}
}  // namespace azer
