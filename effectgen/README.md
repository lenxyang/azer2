
=

----
struct Input {
  Vector4 Position : stream(0), semantic('positon'), offset(0);
  Vector4 Normal;
  Vector2 Texcoord;
};

struct cbUpdate {
  Matrix4 pv;
  Vector4 light_position;
  Vector4 camera_position;
};

VertexLayout Input;
StreamOutLayout Input;
ConstantsBuffer cbUpdate : fragment(0);
ConstantsBuffer cbUpdate : vertex(0);
TextureCube texture : domain(0), fragment(0);
SamplerState sampler : pixel(0);
SamplerState sampler : domain(0), fragment(0);
Entry vsmain : vertex;
Entry psmain : fragement;
----