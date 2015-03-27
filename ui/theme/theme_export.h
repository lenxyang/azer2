#ifndef AZER_UI_THEME_THEME_EXPORT_H_
#define AZER_UI_THEME_THEME_EXPORT_H_

// Defines AZER_THEME_EXPORT so that functionality implemented by the Views module
// can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(AZER_THEME_IMPLEMENTATION)
#define AZER_THEME_EXPORT __declspec(dllexport)
#else
#define AZER_THEME_EXPORT __declspec(dllimport)
#endif  // defined(AZER_THEME_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(AZER_THEME_IMPLEMENTATION)
#define AZER_THEME_EXPORT __attribute__((visibility("default")))
#else
#define AZER_THEME_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define AZER_THEME_EXPORT
#endif

#endif  // AZER_UI_THEME_THEME_EXPORT_H_
