#pragma once

#include "base/macros.h"

namespace azer {
namespace effectgen {
enum class TokenType {
  kNone,
  kStructure,       // struct
  kVertexLayout,    // vertex layout
  kConstantsBuffer, // constants buffer
  kTexture1D,
  kTexture1DArray,
  kTexture2D,
  kTexture2DArray,
  kTextureCube,
  kTexture3D,
  kSamplerState,
  kVertexStage,
  kHullStage,       // hull
  kHullConstants,   // hullconstants
  kDomainStage,
  kGeometryStage,
  kFragmentStage,
  kSemantic,        //
  kOffset,     //
  kStream,          // stream
  kChar,            //
  kString,          //
  kInteger,         //
  kNumber,          //
  kIdentifier,      //
  kTrue,
  kFalse,
  kEOF,
};

struct Token {
  TokenType type;
  std::string data;
  union {
    char   cval;
    int64_t  ival;
    double dval;
    bool   bval;
  };

  Token() : type(TokenType::kNone), ival(0) {}
  Token(TokenType type, const char* data, int64_t v) {
    this->type = type;
    this->data = data;
    this->ival = v;
  }
  Token(TokenType type, const char* data, double v) {
    this->type = type;
    this->data = data;
    this->dval = v;
  }
  Token(TokenType type, const char* data, bool v) {
    this->type = type;
    this->data = data;
    this->bval = v;
  }
};

}  // namespace effectgen
}  // namespace azer
