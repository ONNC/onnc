//===- Value.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_JSON_VALUE_H
#define ONNC_JSON_VALUE_H
#include <onnc/ADT/StringRef.h>
#include <onnc/JSON/Notation.h>
#include <onnc/JSON/Array.h>
#include <onnc/Support/IndentOStream.h>
#include <string>
#include <cstring>

namespace onnc {
namespace json {

class Object;

/** \class Value
 *  \brief The json::Value class encapsulates a value in JSON.
 *
 *  JSON is a format to store structured data. The support in Skymizer  has 7
 *  basic data types @see json::Type:
 *  - bool
 *  - integral number
 *  - floating point number
 *  - string
 *  - object
 *  - array
 *  - null
 *
 *  A value can represent any of the above data types. In addition, json::Value
 *  has a special flag to represent undefined values. This can be queried with
 *  @ref Notation::isUndefined().
 */
class Value : public Notation
{
public:
  Value();

  /// @name integral constructor
  /// @{
  explicit Value(long long int pN);
  explicit Value(long int pN);
  explicit Value(int pN);

  explicit Value(unsigned long long int pN);
  explicit Value(unsigned long int pN);
  explicit Value(unsigned int pN);
  /// @}

  /// @name floating point constructor
  /// @{
  explicit Value(long double pF);
  explicit Value(double pF);
  explicit Value(float pF);
  /// @}

  /// boolean constructor
  explicit Value(bool pB);

  /// @name string constructor
  /// @{
  Value(StringRef pS);
  Value(const char* pS);
  /// @}

  /// @name aggregative constructor
  /// @{
  /// Copy pObject to create a new Value.
  Value(const Object& pObject);

  /// Copy pArray to create a new Value.
  Value(const Array& pArray);
  /// @}

  Value(const Value& pCopy);

  Value(Value&& pOther);

  ~Value();

  Value& operator=(const Value& pValue);

  Value& operator=(Value&& pOther);

  Value& assign(long long int pValue);
  Value& assign(long int pValue);
  Value& assign(int pValue);
  Value& assign(long double pValue);
  Value& assign(double pValue);
  Value& assign(float pValue);
  Value& assign(bool pValue);
  Value& assign(StringRef pValue);
  Value& assign(const char* pValue);
  Value& assign(const std::string& pValue);
  Value& append(const Value& pValue);

  Value& delegate(json::Object& pObject);
  Value& delegate(json::Array& pArray);

  // convenient function for using Value as an Object object.
  // call Object::insert
  bool insert(StringRef pKey, const Value& pValue);

  // convenient function for using Value as an Object object.
  // call Value::write
  bool write(StringRef pKey, const Value& pValue);

  /// clear - reset Value to Unknown.
  void clear();

  void print(IndentOStream& pOS) const;

  /// @name explicit_casting Explicit Casting Functions
  /// @{
  long long int     toInteger()  const { return m_Value.int_p; }
  long double       toFloating() const { return m_Value.float_p; }
  bool              toBool()     const { return m_Value.bool_p; }
  const char*       toString()   const { return m_Value.string_p; }
  const Object&     toObject()   const { return *m_Value.object_p; }
  const Array&      toArray()    const { return *m_Value.array_p; }

  char*   asString() { return m_Value.string_p; }
  Object& asObject() { return *m_Value.object_p; }
  Array&  asArray()  { return *m_Value.array_p; }
  /// @}

private:
  union Holder
  {
  public:
    long double   float_p;
    long long int int_p;
    bool          bool_p;
    char*         string_p;
    Object*       object_p;
    Array*        array_p;
    void*         pointer;

  public:
    Holder() { reset(); }

    ~Holder() { }

    void reset() { ::memset(this, 0, sizeof(Holder)); }
  };

private:
  Holder m_Value;
};

//===----------------------------------------------------------------------===//
// Template Helper
//===----------------------------------------------------------------------===//
template<typename> struct is_value_helper : public std::false_type { };
template<> struct is_value_helper<Value>  : public std::true_type { };

template<typename TP>
struct is_value : public std::integral_constant<bool,(
               is_value_helper<typename std::remove_cv<TP>::type>::value)> { };

} // namespace of json
} // namespace of onnc

#endif
