#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/ui/widget/widget.h"

// This header should be included by code that defines WidgetProperties. It
// should not be included by code that only gets and sets WidgetProperties.
//
// To define a new WidgetProperty:
//
//  #include "foo/foo_export.h"
//  #include "ui/aura/window_property.h"
//
//  DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(FOO_EXPORT, MyType);
//  namespace foo {
//    // Use this to define an exported property that is premitive,
//    // or a pointer you don't want automatically deleted.
//    DEFINE_WINDOW_PROPERTY_KEY(MyType, kMyKey, MyDefault);
//
//    // Use this to define an exported property whose value is a heap
//    // allocated object, and has to be owned and freed by the window.
//    DEFINE_OWNED_WINDOW_PROPERTY_KEY(gfx::Rect, kRestoreBoundsKey, NULL);
//
//    // Use this to define a non exported property that is primitive,
//    // or a pointer you don't want to automatically deleted, and is used
//    // only in a specific file. This will define the property in an unnamed
//    // namespace which cannot be accessed from another file.
//    DEFINE_LOCAL_WINDOW_PROPERTY_KEY(MyType, kMyKey, MyDefault);
//
//  }  // foo namespace
//
// To define a new type used for WidgetProperty.
//
//  // outside all namespaces:
//  DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(FOO_EXPORT, MyType)
//
// If a property type is not exported, use DECLARE_WINDOW_PROPERTY_TYPE(MyType)
// which is a shorthand for DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(, MyType).

namespace azer {
namespace widget {
namespace {

// No single new-style cast works for every conversion to/from int64, so we
// need this helper class. A third specialization is needed for bool because
// MSVC warning C4800 (forcing value to bool) is not suppressed by an explicit
// cast (!).
template<typename T>
class WidgetPropertyCaster {
 public:
  static int64 ToInt64(T x) { return static_cast<int64>(x); }
  static T FromInt64(int64 x) { return static_cast<T>(x); }
};
template<typename T>
class WidgetPropertyCaster<T*> {
 public:
  static int64 ToInt64(T* x) { return reinterpret_cast<int64>(x); }
  static T* FromInt64(int64 x) { return reinterpret_cast<T*>(x); }
};
template<>
class WidgetPropertyCaster<bool> {
 public:
  static int64 ToInt64(bool x) { return static_cast<int64>(x); }
  static bool FromInt64(int64 x) { return x != 0; }
};

}  // namespace

template<typename T>
struct WidgetProperty {
  T default_value;
  const char* name;
  Widget::PropertyDeallocator deallocator;
};

template<typename T>
void Widget::SetProperty(const WidgetProperty<T>* property, T value) {
  int64 old = SetPropertyInternal(
      property,
      property->name,
      value == property->default_value ? NULL : property->deallocator,
      WidgetPropertyCaster<T>::ToInt64(value),
      WidgetPropertyCaster<T>::ToInt64(property->default_value));
  if (property->deallocator &&
      old != WidgetPropertyCaster<T>::ToInt64(property->default_value)) {
    (*property->deallocator)(old);
  }
}

template<typename T>
T Widget::GetProperty(const WidgetProperty<T>* property) const {
  return WidgetPropertyCaster<T>::FromInt64(GetPropertyInternal(
      property, WidgetPropertyCaster<T>::ToInt64(property->default_value)));
}

template<typename T>
void Widget::ClearProperty(const WidgetProperty<T>* property) {
  SetProperty(property, property->default_value);
}

}  // namespace widget
}  // namespace azer

// Macros to instantiate the property getter/setter template functions.
#define DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(EXPORT, T)        \
  template EXPORT void azer::widget::Widget::SetProperty(       \
      const azer::widget::WidgetProperty<T >*, T);              \
  template EXPORT T azer::widget::Widget::GetProperty(          \
      const azer::widget::WidgetProperty<T >*) const;           \
  template EXPORT void azer::widget::Widget::ClearProperty(     \
      const azer::widget::WidgetProperty<T >*);
#define DECLARE_WINDOW_PROPERTY_TYPE(T)         \
  DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(, T)

#define DEFINE_WINDOW_PROPERTY_KEY(TYPE, NAME, DEFAULT)                 \
  COMPILE_ASSERT(sizeof(TYPE) <= sizeof(int64), property_type_too_large); \
  namespace {                                                           \
  const azer::widget::WidgetProperty<TYPE> NAME ## _Value = {DEFAULT, #NAME, NULL}; \
  }                                                                     \
  const azer::widget::WidgetProperty<TYPE>* const NAME = & NAME ## _Value;

#define DEFINE_LOCAL_WINDOW_PROPERTY_KEY(TYPE, NAME, DEFAULT)           \
  COMPILE_ASSERT(sizeof(TYPE) <= sizeof(int64), property_type_too_large); \
  namespace {                                                           \
  const azer::widget::WidgetProperty<TYPE> NAME ## _Value = {DEFAULT, #NAME, NULL}; \
  const azer::widget::WidgetProperty<TYPE>* const NAME = & NAME ## _Value; \
  }

#define DEFINE_OWNED_WINDOW_PROPERTY_KEY(TYPE, NAME, DEFAULT)           \
  namespace {                                                           \
  void Deallocator ## NAME (int64 p) {                                  \
    enum { type_must_be_complete = sizeof(TYPE) };                      \
    delete azer::widget::WidgetPropertyCaster<TYPE*>::FromInt64(p);     \
  }                                                                     \
  const azer::widget::WidgetProperty<TYPE*> NAME ## _Value =            \
  {DEFAULT,#NAME,&Deallocator ## NAME};                                 \
  }                                                                     \
  const azer::widget::WidgetProperty<TYPE*>* const NAME = & NAME ## _Value;


