#pragma once

namespace {
const uint16_t F16S_MASK    = 0x8000;   // f16 sign mask
const uint16_t F16EM_MASK   = 0x7fff;   // f16 exp & mantissa mask
const uint16_t F16MAX       = 0x7bff;   // MAXFLT bit pattern for XMHALF

#define SIGN_EXTEND(x,nb) ((((x)&(1<<((nb)-1)))?((~0)<<(nb)):0)|(x))

// Because these are used in SAL annotations, they need to remain macros rather than const values
#define NUM_PIXELS_PER_BLOCK 16
#define BC6H_MAX_REGIONS 2
#define BC6H_MAX_INDICES 16
#define BC7_MAX_REGIONS 3
#define BC7_MAX_INDICES 16

const size_t BC6H_NUM_CHANNELS = 3;
const size_t BC6H_MAX_SHAPES = 32;

const size_t BC7_NUM_CHANNELS = 4;
const size_t BC7_MAX_SHAPES = 64;

const int32_t BC67_WEIGHT_MAX = 64;
const uint32_t BC67_WEIGHT_SHIFT = 6;
const int32_t BC67_WEIGHT_ROUND = 32;

extern const int g_aWeights2[4];
extern const int g_aWeights3[8];
extern const int g_aWeights4[16];

enum BC_FLAGS {
    BC_FLAGS_NONE       = 0x0,
    BC_FLAGS_DITHER_RGB = 0x10000,  // Enables dithering for RGB colors for BC1-3
    BC_FLAGS_DITHER_A   = 0x20000,  // Enables dithering for Alpha channel for BC1-3
    BC_FLAGS_UNIFORM    = 0x40000,  // By default, uses perceptual weighting for BC1-3; this flag makes it a uniform weighting
    BC_FLAGS_USE_3SUBSETS = 0x80000,// By default, BC7 skips mode 0 & 2; this flag adds those modes back
};


}  // namespace
