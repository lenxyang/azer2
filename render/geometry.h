#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {
class Matrix4;
class MeshPart;
class IndicesData;
class SlotVertexData;
class VertexDesc;

typedef scoped_refptr<MeshPart> MeshPartPtr;

// sphere
struct GeoSphereParams {
  float radius;
  int32 stack;
  int32 slice;
};
AZER_EXPORT MeshPartPtr CreateSphereMeshPart(
    VertexDesc* desc, const GeoSphereParams& params);
AZER_EXPORT MeshPartPtr CreateSphereMeshPart(
    VertexDesc* desc, const Matrix4& transform, const GeoSphereParams& params);
AZER_EXPORT MeshPartPtr CreateSphereFrameMeshPart(
    VertexDesc* desc, const GeoSphereParams& params);
AZER_EXPORT MeshPartPtr CreateSphereFrameMeshPart(
    VertexDesc* desc, const Matrix4& transform, const GeoSphereParams& params);

// box
AZER_EXPORT MeshPartPtr CreateBoxMeshPart(VertexDesc* desc);
AZER_EXPORT MeshPartPtr CreateBoxMeshPart(
    VertexDesc* desc, const Matrix4& transform);
AZER_EXPORT MeshPartPtr CreateBoxFrameMeshPart(VertexDesc* desc);
AZER_EXPORT MeshPartPtr CreateBoxFrameMeshPart(
    VertexDesc* desc, const Matrix4& transform);

// plane
struct GeoPlaneParams {
  int32 row;
  int32 column;
  float row_width;
  float column_width;
};
AZER_EXPORT MeshPartPtr CreatePlaneMeshPart(
    VertexDesc* desc, const GeoPlaneParams& params);
AZER_EXPORT MeshPartPtr CreatePlaneMeshPart(
    VertexDesc* desc, const Matrix4& transform, const GeoPlaneParams& params);
AZER_EXPORT MeshPartPtr CreatePlaneFrameMeshPart(
    VertexDesc* desc, const GeoPlaneParams& params);
AZER_EXPORT MeshPartPtr CreatePlaneFrameMeshPart(
    VertexDesc* desc, const Matrix4& transform, const GeoPlaneParams& params);

// round
AZER_EXPORT MeshPartPtr CreateRoundMeshPart(
    VertexDesc* desc, float radius, int32 slice);
AZER_EXPORT MeshPartPtr CreateRoundMeshPart(
    VertexDesc* desc, const Matrix4& transform, float radius, int32 slice);
AZER_EXPORT MeshPartPtr CreateRoundFrameMeshPart(
    VertexDesc* desc, float radius, int32 slice);
AZER_EXPORT MeshPartPtr CreateRoundFrameMeshPart(
    VertexDesc* desc, const Matrix4& transform, float radius, int32 slice);

//
struct GeoConeParams {
  float height;
  float radius;
  int slice;
};
AZER_EXPORT MeshPartPtr CreateConeMeshPart(
    VertexDesc* desc, const GeoConeParams& params);
AZER_EXPORT MeshPartPtr CreateConeMeshPart(
    VertexDesc* desc, const Matrix4& transform, const GeoConeParams& params);
AZER_EXPORT MeshPartPtr CreateTaperMeshPart(
    VertexDesc* desc, const GeoConeParams& params);
AZER_EXPORT MeshPartPtr CreateTaperMeshPart(
    VertexDesc* desc, const Matrix4& transform, const GeoConeParams& params);

struct GeoBarrelParams {
  float top_radius;
  float bottom_radius;
  float height;
  int32 slice;
  int32 stack;
};

AZER_EXPORT MeshPartPtr CreateBarrelMeshPart(
    VertexDesc* desc, const Matrix4& transform, const GeoBarrelParams& params);
AZER_EXPORT MeshPartPtr CreateBarrelMeshPart(
    VertexDesc* desc, const GeoBarrelParams& params);

AZER_EXPORT MeshPartPtr CreateCylinderMeshPart(
    VertexDesc* desc, const Matrix4& transform, const GeoBarrelParams& params);
AZER_EXPORT MeshPartPtr CreateCylinderMeshPart(
    VertexDesc* desc, const GeoBarrelParams& params);

// util
AZER_EXPORT void CalcIndexedTriangleNormal(SlotVertexData* vbd, IndicesData* idata);
AZER_EXPORT void CalcTriangleNormal(SlotVertexData* vbd, int* indices);
}  // namespace azer
