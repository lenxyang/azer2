#pragma once

#include "azer/math/math.h"

namespace azer {
class HDRColorA;
class LDRColorA {
 public:
  uint8_t r, g, b, a;

  LDRColorA();
  LDRColorA(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) 
      : r(_r), g(_g), b(_b), a(_a) {}

  const uint8& operator [] (size_t ele) const;
  uint8& operator [] (size_t ele);

  LDRColorA operator = (const HDRColorA& c);
  static void InterpolateRGB(const LDRColorA& c0, const LDRColorA& c1, size_t wc, 
                             size_t wcprec, LDRColorA* out);
  static void InterpolateA(const LDRColorA& c0, const LDRColorA& c1, size_t wc,
                           size_t waprec, LDRColorA* out);
  static void Interpolate(const LDRColorA& c0, const LDRColorA& c1, size_t wc, 
                          size_t wa, size_t wcprec, size_t waprec, LDRColorA* out);
};

class HDRColorA {
 public:
  float r, g, b, a;

 public:
  HDRColorA() {}
  HDRColorA(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
  HDRColorA(const HDRColorA& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
  HDRColorA(const LDRColorA& c);

  // binary operators
  HDRColorA operator + (const HDRColorA& c) const;
  HDRColorA operator - (const HDRColorA& c) const;
  HDRColorA operator * (float f) const;
  HDRColorA operator / (float f) const;
  float operator * (const HDRColorA& c) const;

  // assignment operators
  HDRColorA& operator += (const HDRColorA& c);
  HDRColorA& operator -= (const HDRColorA& c);
  HDRColorA& operator *= (float f);
  HDRColorA& operator /= (float f);
  HDRColorA& operator = (const LDRColorA& c);
  HDRColorA& Clamp(float fMin,  float fMax);
  LDRColorA ToLDRColorA() const;
};

class INTColor {
 public:
  int r, g, b;
  int pad;

 public:
  INTColor() {}
  INTColor(int nr, int ng, int nb) {r = nr; g = ng; b = nb;}
  INTColor(const INTColor& c) {r = c.r; g = c.g; b = c.b;}

  INTColor operator - (const INTColor& c) const;
  INTColor& operator += (const INTColor& c);
  INTColor& operator -= (const INTColor& c);
  INTColor& operator &= (const INTColor& c);
  int& operator [] (uint8_t i);

  void Set(const HDRColorA& c,  bool bSigned);
  INTColor& Clamp(int iMin,  int iMax);
  INTColor& SignExtend( const LDRColorA& Prec);
  void ToF16(uint16_t aF16[3],  bool bSigned) const;
 private:    
  static int F16ToINT(const uint16_t f, bool bsigned);
  static uint16_t INT2F16(int input,  bool bsigned);
};
}  // namespace azer
