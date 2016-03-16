
float3 TBNToWorldSpace(float3 normal, float3 T, float3 N, float3 B) {
  float3 n = normal.xyz * 2.0 - 1.0f;
  float3 r = float3(0.0f, 0.0, 0.0);
  r.x = dot(T, n);
  r.y = dot(B, n);
  r.z = dot(N, n);
  return r;
}
