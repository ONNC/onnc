//===- Type.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_JSON_TYPE_H
#define ONNC_JSON_TYPE_H
#include <type_traits>
#include <string>
#include <onnc/ADT/StringRef.h>

namespace onnc {
namespace json {

/** \enum json::Type
 *
 *  JSON is a format to store structured data. JSON in Skymizer has 8 basic
 *  data types:
 *  - boolean
 *  - integral number
 *  - floating point number
 *  - string
 *  - array
 *  - object
 *  - null
 *  - undefined
 *
 *  A value can have any of above types. A boolean value is represented by
 *  the string 'true' or 'false in JSON. JSON doesn't explicitly specify the
 *  valid range for numbers, but the support in Skymizer is limited to the
 *  valid range and precision of 'long long' and 'long double'. An array is a
 *  list of values and an object is a set of key/value pairs. All keys in an
 *  object are strings. A null value is represented by the string 'null' in
 *  JSON. Any value without initial value is undefined value.
 */
enum Type {
  INT,    ///< JSON's int
  FLOAT,  ///< JSON's float 3.14 12e-10
  BOOL,   ///< JSON's boolean (true, false)
  STRING, ///< JSON's string " ... " or (not really JSON) ' ... '
  OBJECT, ///< JSON's object { ... }
  ARRAY,  ///< JSON's array [ ... ]
  NIL,    ///< JSON's null
  UNDEF   ///< Undefined
};

//===----------------------------------------------------------------------===//
// Helper functions for type checking
//===----------------------------------------------------------------------===//
template<typename> struct is_integral_helper : public std::false_type { };
template<typename> struct is_floating_helper : public std::false_type { };
template<typename> struct is_bool_helper     : public std::false_type { };
template<typename> struct is_string_helper   : public std::false_type { };

template<> struct is_integral_helper<long long int> : public std::true_type { };
template<> struct is_integral_helper<long int>      : public std::true_type { };
template<> struct is_integral_helper<int>           : public std::true_type { };

template<> struct is_floating_helper<long double>   : public std::true_type { };
template<> struct is_floating_helper<double>        : public std::true_type { };
template<> struct is_floating_helper<float>         : public std::true_type { };

template<> struct is_bool_helper<bool>              : public std::true_type { };

template<> struct is_string_helper<const char*>     : public std::true_type { };
template<> struct is_string_helper<std::string>     : public std::true_type { };
template<> struct is_string_helper<StringRef>       : public std::true_type { };

//===----------------------------------------------------------------------===//
// JSON Type Checking
//===----------------------------------------------------------------------===//
/// \struct is_integral
/// \brief Trait class that identifies whether TP is an integral type in JSON
///
/// \code{.cpp}
/// using namespace onnc;
///
/// std::cout << json::is_integral<char>::value   << std::endl; /* false */
/// std::cout << json::is_integral<double>::value << std::endl; /* false */
/// std::cout << json::is_integral<long>::value   << std::endl; /* true */
/// \endcode
template<typename TP>
struct is_integral : public std::integral_constant<bool,(
                              is_integral_helper<
                                typename std::remove_cv<TP>::type
                              >::value)> { };

/// \struct is_floating
/// \brief Trait class that identifies whether TP is an floating point type
/// in JSON
///
/// \code{.cpp}
/// using namespace onnc;
///
/// std::cout << json::is_floating<char>::value   << std::endl; /* false */
/// std::cout << json::is_floating<double>::value << std::endl; /* true */
/// std::cout << json::is_floating<long>::value   << std::endl; /* false */
/// \endcode
template<typename TP>
struct is_floating : public std::integral_constant<bool,(
                              is_floating_helper<
                                typename std::remove_cv<TP>::type
                              >::value)> { };

/// \struct is_bool
/// \brief Trait class that identifies whether TP is an boolean type in JSON
///
/// \code{.cpp}
/// using namespace onnc;
///
/// std::cout << json::is_bool<bool>::value   << std::endl; /* true */
/// std::cout << json::is_bool<double>::value << std::endl; /* false */
/// \endcode
template<typename TP>
struct is_bool : public std::integral_constant<bool,(
                              is_bool_helper<
                                typename std::remove_cv<TP>::type
                              >::value)> { };

/// \struct is_string
/// \brief Trait class that identifies whether TP is an string type in JSON
///
/// \code{.cpp}
/// using namespace onnc;
///
/// std::cout << json::is_string<long long>::value   << std::endl; /* false */
/// std::cout << json::is_string<StringRef>::value   << std::endl; /* true */
/// std::cout << json::is_string<std::string>::value << std::endl; /* true */
/// std::cout << json::is_string<const char*>::value << std::endl; /* true */
/// \endcode
template<typename TP>
struct is_string : public std::integral_constant<bool,(
                              is_string_helper<
                                typename std::remove_cv<TP>::type
                              >::value)> { };

} // namespace of json
} // namespace of onnc

#endif
