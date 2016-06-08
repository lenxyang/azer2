#include "azer/util/images/color.h"

#include <algorithm>
#include "base/logging.h"
#include "azer/util/images/block_compress.h"

namespace azer {

extern const int g_aWeights2[4];
extern const int g_aWeights3[8];
extern const int g_aWeights4[16];

// class LDRColorA
void LDRColorA::InterpolateRGB(const LDRColorA& c0, const LDRColorA& c1, size_t wc, 
                               size_t wcprec, LDRColorA* out) {
  const int* aWeights = nullptr;
  switch(wcprec)
  {
    case 2: 
      aWeights = g_aWeights2; 
      DCHECK(wc < 4); 
      _Analysis_assume_(wc < 4); 
      break;
    case 3: 
      aWeights = g_aWeights3;
      DCHECK(wc < 8);
      _Analysis_assume_(wc < 8); 
      break;
    case 4: aWeights = g_aWeights4;
      DCHECK(wc < 16); 
      _Analysis_assume_(wc < 16); 
      break;
    default: 
      NOTREACHED(); 
      out->r = out->g = out->b = 0; 
      return;
  }
  out->r = uint8_t((uint32_t(c0.r) * uint32_t(BC67_WEIGHT_MAX - aWeights[wc]) 
                  + uint32_t(c1.r) * uint32_t(aWeights[wc]) 
                  + BC67_WEIGHT_ROUND) >> BC67_WEIGHT_SHIFT);
  out->g = uint8_t((uint32_t(c0.g) * uint32_t(BC67_WEIGHT_MAX - aWeights[wc]) 
                  + uint32_t(c1.g) * uint32_t(aWeights[wc]) 
                  + BC67_WEIGHT_ROUND) >> BC67_WEIGHT_SHIFT);
  out->b = uint8_t((uint32_t(c0.b) * uint32_t(BC67_WEIGHT_MAX - aWeights[wc]) 
                  + uint32_t(c1.b) * uint32_t(aWeights[wc]) 
                  + BC67_WEIGHT_ROUND) >> BC67_WEIGHT_SHIFT);
}

void LDRColorA::InterpolateA(const LDRColorA& c0, const LDRColorA& c1, size_t wa, 
                             size_t waprec, LDRColorA* out) {
  const int* aWeights = nullptr;
  switch(waprec) {
    case 2: 
      aWeights = g_aWeights2;
      DCHECK(wa < 4); 
      _Analysis_assume_(wa < 4); break;
    case 3: 
      aWeights = g_aWeights3;
      DCHECK(wa < 8);
      _Analysis_assume_(wa < 8);
      break;
    case 4:
      aWeights = g_aWeights4;
      DCHECK(wa < 16);
      _Analysis_assume_(wa < 16);
      break;
    default: 
      NOTREACHED(); 
      out->a = 0; 
      return;
  }
  out->a = uint8_t((uint32_t(c0.a) * uint32_t(BC67_WEIGHT_MAX - aWeights[wa])
                  + uint32_t(c1.a) * uint32_t(aWeights[wa]) 
                  + BC67_WEIGHT_ROUND) >> BC67_WEIGHT_SHIFT);
}

void LDRColorA::Interpolate(const LDRColorA& c0, const LDRColorA& c1, 
                            size_t wc, size_t wa, size_t wcprec, size_t waprec, 
                            LDRColorA* out) {
  InterpolateRGB(c0, c1, wc, wcprec, out);
  InterpolateA(c0, c1, wa, waprec, out);
}

const uint8_t& LDRColorA::operator [] (size_t ele) const {
  switch(ele) {
    case 0: return r;
    case 1: return g;
    case 2: return b;
    case 3: return a;
    default: NOTREACHED(); return r;
  }
}

uint8_t& LDRColorA::operator [] (size_t ele) {
  switch(ele) {
    case 0: return r;
    case 1: return g;
    case 2: return b;
    case 3: return a;
    default: NOTREACHED(); return r;
  }
}

LDRColorA LDRColorA::operator = (const HDRColorA& c) {
  LDRColorA ret;
  HDRColorA tmp(c);
  tmp = tmp.Clamp(0.0f, 1.0f) * 255.0f;
  ret.r = uint8_t(tmp.r + 0.001f);
  ret.g = uint8_t(tmp.g + 0.001f);
  ret.b = uint8_t(tmp.b + 0.001f);
  ret.a = uint8_t(tmp.a + 0.001f);
  return ret;
}

// class HDRColorA
HDRColorA::HDRColorA(const LDRColorA& c) {
  r = float(c.r) * (1.0f/255.0f);
  g = float(c.g) * (1.0f/255.0f);
  b = float(c.b) * (1.0f/255.0f);
  a = float(c.a) * (1.0f/255.0f);
}

// binary operators
HDRColorA HDRColorA::operator + (const HDRColorA& c) const {
  return HDRColorA(r + c.r, g + c.g, b + c.b, a + c.a);
}

HDRColorA HDRColorA::operator - (const HDRColorA& c) const {
  return HDRColorA(r - c.r, g - c.g, b - c.b, a - c.a);
}

HDRColorA HDRColorA::operator * (float f) const {
  return HDRColorA(r * f, g * f, b * f, a * f);
}

HDRColorA HDRColorA::operator / (float f) const {
  float fInv = 1.0f / f;
  return HDRColorA(r * fInv, g * fInv, b * fInv, a * fInv);
}

float HDRColorA::operator * (const HDRColorA& c) const {
  return r * c.r + g * c.g + b * c.b + a * c.a;
}

// assignment operators
HDRColorA& HDRColorA::operator += (const HDRColorA& c) {
  r += c.r;
  g += c.g;
  b += c.b;
  a += c.a;
  return *this;
}

HDRColorA& HDRColorA::operator -= (const HDRColorA& c) {
  r -= c.r;
  g -= c.g;
  b -= c.b;
  a -= c.a;
  return *this;
}

HDRColorA& HDRColorA::operator *= (float f) {
  r *= f;
  g *= f;
  b *= f;
  a *= f;
  return *this;
}

HDRColorA& HDRColorA::operator /= (float f) {
  float fInv = 1.0f / f;
  r *= fInv;
  g *= fInv;
  b *= fInv;
  a *= fInv;
  return *this;
}

HDRColorA& HDRColorA::operator = (const LDRColorA& c) {
  r = (float) c.r;
  g = (float) c.g;
  b = (float) c.b;
  a = (float) c.a;
  return *this;
}

HDRColorA& HDRColorA::Clamp(float fMin,  float fMax) {
  r = std::min<float>(fMax, std::max<float>(fMin, r));
  g = std::min<float>(fMax, std::max<float>(fMin, g));
  b = std::min<float>(fMax, std::max<float>(fMin, b));
  a = std::min<float>(fMax, std::max<float>(fMin, a));
  return *this;
}

LDRColorA HDRColorA::ToLDRColorA() const {
  return LDRColorA((uint8_t) (r + 0.01f), (uint8_t) (g + 0.01f), 
                   (uint8_t) (b + 0.01f), (uint8_t) (a + 0.01f));
}

// INT Color
INTColor INTColor::operator - (const INTColor& c) const {
  return INTColor(r - c.r, g - c.g, b - c.b);
}

INTColor& INTColor::operator += (const INTColor& c) {
  r += c.r;
  g += c.g;
  b += c.b;
  return *this;
}

INTColor& INTColor::operator -= (const INTColor& c) {
  r -= c.r;
  g -= c.g;
  b -= c.b;
  return *this;
}

INTColor& INTColor::operator &= (const INTColor& c) {
  r &= c.r;
  g &= c.g;
  b &= c.b;
  return *this;
}

int& INTColor::operator [] (uint8_t i)  {
  DCHECK(i < sizeof(INTColor) / sizeof(int));
  _Analysis_assume_(i < sizeof(INTColor) / sizeof(int));
  return ((int*) this)[i];
}

void INTColor::Set(const HDRColorA& c, bool bSigned) {
  r = F16ToINT(c.r, bSigned);
  g = F16ToINT(c.g, bSigned);
  b = F16ToINT(c.b, bSigned);
}

INTColor& INTColor::Clamp(int iMin, int iMax) {
  r = std::min<int>(iMax, std::max<int>(iMin, r));
  g = std::min<int>(iMax, std::max<int>(iMin, g));
  b = std::min<int>(iMax, std::max<int>(iMin, b));
  return *this;
}

INTColor& INTColor::SignExtend(const LDRColorA& Prec) {
  r = SIGN_EXTEND(r, Prec.r);
  g = SIGN_EXTEND(g, Prec.g);
  b = SIGN_EXTEND(b, Prec.b);
  return *this;
}

void INTColor::ToF16(uint16_t aF16[3], bool bSigned) const {
  aF16[0] = INT2F16(r, bSigned);
  aF16[1] = INT2F16(g, bSigned);
  aF16[2] = INT2F16(b, bSigned);
}

int INTColor::F16ToINT(uint16_t input, bool bSigned) {
  int out, s;
  if(bSigned) {
    s = input & F16S_MASK;
    input &= F16EM_MASK;
    if(input > F16MAX) 
      out = F16MAX;
    else 
      out = input;
    out = s ? -out : out;
  } else {
    if(input & F16S_MASK) 
      out = 0;
    else 
      out = input;
  }
  return out;
}

uint16_t INTColor::INT2F16(int input, bool bSigned) {
  uint16_t out;
  if(bSigned) {
    int s = 0;
    if(input < 0) {
      s = F16S_MASK;
      input = -input;
    }
    out = uint16_t(s | input);
  } else {
    CHECK(input >= 0 && input <= F16MAX);
    out = (uint16_t) input;
  }

  return out;
}

}  // namespace azer
