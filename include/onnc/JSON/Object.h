//===- Object.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_JSON_OBJECT_H
#define ONNC_JSON_OBJECT_H
#include <onnc/ADT/StringMap.h>
#include <onnc/JSON/Value.h>
#include <onnc/JSON/Notation.h>
#include <ostream>

namespace onnc {
namespace json {

//===----------------------------------------------------------------------===//
// Template Helper
//===----------------------------------------------------------------------===//
template<typename> struct is_object_helper : public std::false_type { };
template<> struct is_object_helper<Object> : public std::true_type { };

template<typename TP>
struct is_object : public std::integral_constant<bool,(
               is_object_helper<typename std::remove_cv<TP>::type>::value)> { };

/** \class Object
 *  \brief A JSON object
 *
 *  An object is an unordered set of name/value pairs. An object begins with
 *  { (left brace) and ends with } (right brace). Each name is followed by
 *  : (colon) and the name/value pairs are separated by , (comma).
 */
class Object : public StringMap<Value>, public Notation
{
public:
  typedef StringMap<Value>::const_iterator const_iterator;
  typedef StringMap<Value>::iterator       iterator;

public:
  Object();

  Object(const Object& pCopy);

  /// Move constructor
  Object(Object&& pOther);

  Object& operator=(const Object& pCopy);

  /// Move assignment operator
  Object& operator=(Object&& pOther);

  ~Object() { }

  bool hasValue(StringRef pKey) const;

  const Value& get(StringRef pKey) const;

  Value& get(StringRef pKey);

  /// Operator override to get value with a key
  /// \code
  /// Value v = obj["name"];
  /// \endcode
  const Value& operator[](StringRef pKey) const;

  /// Override operator to assign value with a key
  /// \code
  /// obj["name"] = Value("test");
  /// \endcode
  Value& operator[](StringRef pKey);

  /// Insert a new @ref pKey with value @ref pValue.
  /// If key already exists, then value won't be written.
  /// @retval false The key exists.
  bool insert(StringRef pKey, const Value& pValue);

  /// Insert a new @ref pKey with value @ref pValue.
  /// If key already exists, then value won't be written.
  /// @retval false The key exists.
  template<typename T> bool
  insert(StringRef pKey, const T& pValue)
  { return insert(pKey, json::Value(pValue)); }

  /// Override @ref pKey with value @ref pValue
  /// Even if key already exists, value will be written.
  /// @retval false The key exists
  bool write(StringRef pKey, const Value& pValue);

  /// Override @ref pKey with value @ref pValue
  /// Even if key already exists, value will be written.
  /// @retval false The key exists
  template<typename T> bool
  write(StringRef pKey, const T& pValue)
  { return write(pKey, json::Value(pValue)); }

  /// print the object to @ref pOS
  void print(IndentOStream& pOS) const;

  /// @return The number of entries.
  unsigned int size() const { return numOfEntries(); }
};

} // namespace of json
} // namespace of onnc

#endif
