//===- Array.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_JSON_ARRAY_H
#define ONNC_JSON_ARRAY_H
#include <onnc/JSON/Notation.h>
#include <onnc/Support/IndentOStream.h>
#include <vector>

namespace onnc {
namespace json {

class Value;

/** \class Array
 *  \brief A JSON Array
 *
 *  An array is an ordered collection of values. An array begins with
 *  [ (left bracket) and ends with ] (right bracket). Values are separated by
 *  , (comma).
 *
 *  We don't use typedef or alias here because C++ disallows forward declaration
 *  of a typedef or an alias.
 */
class Array : public std::vector<Value>, public Notation
{
public:
  Array();

  ~Array() { }

  Array(const Array& pArray);

  Array& operator=(const Array& pArray);

  void print(IndentOStream& pOS) const;
};

//===----------------------------------------------------------------------===//
// Template Helper
//===----------------------------------------------------------------------===//
template<typename> struct is_array_helper : public std::false_type { };
template<> struct is_array_helper<Array>  : public std::true_type { };

template<typename TP>
struct is_array : public std::integral_constant<bool,(
                is_array_helper<typename std::remove_cv<TP>::type>::value)> { };

} // namespace json
} // namespace onnc

#endif
