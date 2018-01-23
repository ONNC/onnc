//===- Notation.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_JSON_NOTATION_H
#define ONNC_JSON_NOTATION_H
#include <onnc/JSON/Type.h>

namespace onnc {
namespace json {

/** \class Notation
 *  \brief A json Notation is the common base for various json data types.
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
 *  @ref isUndefined().
 */
class Notation
{
public:
  Notation();

  Notation(Type pType);

  Notation(const Notation& pCopy);

  Notation(Notation&& pOther);

  virtual ~Notation() { }

  Notation& operator=(const Notation& pCopy);

  Notation& operator=(Notation&& pCopy);

  Type type() const { return m_Type; }

  bool isInteger()  const { return (INT == m_Type); }
  bool isFloating()  const { return (FLOAT == m_Type); }
  bool isBool()      const { return (BOOL == m_Type); }
  bool isString()    const { return (STRING == m_Type); }
  bool isObject()    const { return (OBJECT == m_Type); }
  bool isArray()     const { return (ARRAY == m_Type); }
  bool isNull()      const { return (NIL == m_Type); }
  bool isUndefined() const { return (UNDEF == m_Type); }

protected:
  void setType(Type pType) { m_Type = pType; }

protected:
  Type m_Type;
};

} // namespace of json
} // namespace of onnc

#endif
