#pragma once

// Defines WIDGET_EXPORT so that functionality implemented by the Views module
// can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(WIDGET_IMPLEMENTATION)
#define WIDGET_EXPORT __declspec(dllexport)
#else
#define WIDGET_EXPORT __declspec(dllimport)
#endif  // defined(WIDGET_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(WIDGET_IMPLEMENTATION)
#define WIDGET_EXPORT __attribute__((visibility("default")))
#else
#define WIDGET_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define WIDGET_EXPORT
#endif

