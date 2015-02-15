#pragma once

// Defines SKINNED_EXPORT so that functionality implemented by the Views module
// can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(SKINNED_IMPLEMENTATION)
#define SKINNED_EXPORT __declspec(dllexport)
#else
#define SKINNED_EXPORT __declspec(dllimport)
#endif  // defined(SKINNED_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(SKINNED_IMPLEMENTATION)
#define SKINNED_EXPORT __attribute__((visibility("default")))
#else
#define SKINNED_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define SKINNED_EXPORT
#endif

