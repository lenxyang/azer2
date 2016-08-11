
StructuredBuffer<float> input0 : register(t0);
StructuredBuffer<float> input1 : register(t1);
RWStructuredBuffer<float> output : register(u0);

float DownScale4x4(uint2 cur) {
  float avg = 0.0f;
  int3 texcoord = int3(cur * 4, 0);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      int index = (cur.y + j) * 64 + cur.x;
      avg += input0.Load(index) + input1.Load(index);
    }
  }

  avg /= 16.0f;
  return avg;
}

[numthreads(16, 16, 1)]
void cs_main(uint3 groupid : SV_GroupID,
            uint3 dispatch_threadid : SV_DispatchThreadID,
            uint3 group_threadid : SV_GroupThreadID) {
  uint2 cur = uint2(dispatch_threadid.x * 16, dispatch_threadid.y * 16);
  float avg = DownScale4x4(cur);
  int index = dispatch_threadid.y * 16 + dispatch_threadid.x;
  output[index] = avg;
}

