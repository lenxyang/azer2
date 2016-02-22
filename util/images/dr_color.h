#pragma once

namespace azer {
class LDRColorA
{
 public:
  uint8_t r, g, b, a;

  LDRColorA() DIRECTX_CTOR_DEFAULT
      LDRColorA(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) : r(_r), g(_g), b(_b), a(_a) {}

  const uint8_t& operator [] (_In_range_(0,3) size_t uElement) const
  {
    switch(uElement)
    {
      case 0: return r;
      case 1: return g;
      case 2: return b;
      case 3: return a;
      default: assert(false); return r;
    }
  }

  uint8_t& operator [] (_In_range_(0,3) size_t uElement)
  {
    switch(uElement)
    {
      case 0: return r;
      case 1: return g;
      case 2: return b;
      case 3: return a;
      default: assert(false); return r;
    }
  }

  LDRColorA operator = (_In_ const HDRColorA& c);
  static void InterpolateRGB(_In_ const LDRColorA& c0, _In_ const LDRColorA& c1, _In_ size_t wc, _In_ _In_range_(2, 4) size_t wcprec, _Out_ LDRColorA& out)
  {
    const int* aWeights = nullptr;
    switch(wcprec)
    {
      case 2: aWeights = g_aWeights2; assert( wc < 4 ); _Analysis_assume_( wc < 4 ); break;
      case 3: aWeights = g_aWeights3; assert( wc < 8 ); _Analysis_assume_( wc < 8 ); break;
      case 4: aWeights = g_aWeights4; assert( wc < 16 ); _Analysis_assume_( wc < 16 ); break;
      default: assert(false); out.r = out.g = out.b = 0; return;
    }
    out.r = uint8_t((uint32_t(c0.r) * uint32_t(BC67_WEIGHT_MAX - aWeights[wc]) + uint32_t(c1.r) * uint32_t(aWeights[wc]) + BC67_WEIGHT_ROUND) >> BC67_WEIGHT_SHIFT);
    out.g = uint8_t((uint32_t(c0.g) * uint32_t(BC67_WEIGHT_MAX - aWeights[wc]) + uint32_t(c1.g) * uint32_t(aWeights[wc]) + BC67_WEIGHT_ROUND) >> BC67_WEIGHT_SHIFT);
    out.b = uint8_t((uint32_t(c0.b) * uint32_t(BC67_WEIGHT_MAX - aWeights[wc]) + uint32_t(c1.b) * uint32_t(aWeights[wc]) + BC67_WEIGHT_ROUND) >> BC67_WEIGHT_SHIFT);
  }

  static void InterpolateA(_In_ const LDRColorA& c0, _In_ const LDRColorA& c1, _In_ size_t wa, _In_range_(2, 4) _In_ size_t waprec, _Out_ LDRColorA& out)
  {
    const int* aWeights = nullptr;
    switch(waprec)
    {
      case 2: aWeights = g_aWeights2; assert( wa < 4 ); _Analysis_assume_( wa < 4 ); break;
      case 3: aWeights = g_aWeights3; assert( wa < 8 ); _Analysis_assume_( wa < 8 ); break;
      case 4: aWeights = g_aWeights4; assert( wa < 16 ); _Analysis_assume_( wa < 16 ); break;
      default: assert(false); out.a = 0; return;
    }
    out.a = uint8_t((uint32_t(c0.a) * uint32_t(BC67_WEIGHT_MAX - aWeights[wa]) + uint32_t(c1.a) * uint32_t(aWeights[wa]) + BC67_WEIGHT_ROUND) >> BC67_WEIGHT_SHIFT);
  }

  static void Interpolate(_In_ const LDRColorA& c0, _In_ const LDRColorA& c1, _In_ size_t wc, _In_ size_t wa, _In_ _In_range_(2, 4) size_t wcprec, _In_ _In_range_(2, 4) \
                          size_t waprec, _Out_ LDRColorA& out)
  {
    InterpolateRGB(c0, c1, wc, wcprec, out);
    InterpolateA(c0, c1, wa, waprec, out);
  }
};

class HDRColorA {
 public:
  float r, g, b, a;

 public:
  HDRColorA() {}
  HDRColorA(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
  HDRColorA(const HDRColorA& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
  HDRColorA(const LDRColorA& c) {
    r = float(c.r) * (1.0f/255.0f);
    g = float(c.g) * (1.0f/255.0f);
    b = float(c.b) * (1.0f/255.0f);
    a = float(c.a) * (1.0f/255.0f);
  }

  // binary operators
  HDRColorA operator + (const HDRColorA& c const {
    return HDRColorA(r + c.r, g + c.g, b + c.b, a + c.a);
  }

  HDRColorA operator - (const HDRColorA& c const {
    return HDRColorA(r - c.r, g - c.g, b - c.b, a - c.a);
  }

  HDRColorA operator * (float f const {
    return HDRColorA(r * f, g * f, b * f, a * f);
  }

  HDRColorA operator / (float f const {
    float fInv = 1.0f / f;
    return HDRColorA(r * fInv, g * fInv, b * fInv, a * fInv);
  }

  float operator * (const HDRColorA& c const {
    return r * c.r + g * c.g + b * c.b + a * c.a;
  }

  // assignment operators
  HDRColorA& operator += (const HDRColorA& c {
    r += c.r;
    g += c.g;
    b += c.b;
    a += c.a;
    return *this;
  }

  HDRColorA& operator -= (const HDRColorA& c {
    r -= c.r;
    g -= c.g;
    b -= c.b;
    a -= c.a;
    return *this;
  }

  HDRColorA& operator *= (float f {
    r *= f;
    g *= f;
    b *= f;
    a *= f;
    return *this;
  }

  HDRColorA& operator /= (float f {
    float fInv = 1.0f / f;
    r *= fInv;
    g *= fInv;
    b *= fInv;
    a *= fInv;
    return *this;
  }

  HDRColorA& operator = ( const LDRColorA& c) {
    r = (float) c.r;
    g = (float) c.g;
    b = (float) c.b;
    a = (float) c.a;
    return *this;
  }

  HDRColorA& Clamp( float fMin,  float fMax) {
    r = std::min<float>(fMax, std::max<float>(fMin, r));
    g = std::min<float>(fMax, std::max<float>(fMin, g));
    b = std::min<float>(fMax, std::max<float>(fMin, b));
    a = std::min<float>(fMax, std::max<float>(fMin, a));
    return *this;
  }

  LDRColorA ToLDRColorA() const {
    return LDRColorA((uint8_t) (r + 0.01f), (uint8_t) (g + 0.01f), 
                     (uint8_t) (b + 0.01f), (uint8_t) (a + 0.01f));
  }
};

inline LDRColorA LDRColorA::operator = ( const HDRColorA& c) {
  LDRColorA ret;
  HDRColorA tmp(c);
  tmp = tmp.Clamp(0.0f, 1.0f) * 255.0f;
  ret.r = uint8_t(tmp.r + 0.001f);
  ret.g = uint8_t(tmp.g + 0.001f);
  ret.b = uint8_t(tmp.b + 0.001f);
  ret.a = uint8_t(tmp.a + 0.001f);
  return ret;
}
}  // namespace azer
