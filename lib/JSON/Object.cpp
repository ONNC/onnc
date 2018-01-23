//===- Object.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/JSON/Object.h>
#include <onnc/Diagnostic/MsgHandling.h>

using namespace onnc;
using namespace onnc::json;

//===----------------------------------------------------------------------===//
// Object
//===----------------------------------------------------------------------===//
Object::Object()
  : StringMap<Value>(4 /** magic number **/), Notation(OBJECT) {
}

Object::Object(const Object& pCopy)
 : StringMap<Value>(pCopy), Notation(OBJECT) {
}

Object::Object(Object&& pOther)
  : StringMap<Value>(std::move(pOther)), Notation(std::move(pOther)) {
}

Object& Object::operator=(const Object& pCopy)
{
  StringMap<Value>::operator=(pCopy);
  Notation::operator=(pCopy);
  return *this;
}

Object& Object::operator=(Object&& pOther)
{
  if (this != &pOther) {
    Notation::operator=(std::move(pOther));
    StringMap<Value>::operator=(std::move(pOther));
  }
  return *this;
}

bool Object::hasValue(StringRef pKey) const
{
  const_iterator value = find(pKey);
  return (end() != value);
}

Value& Object::get(StringRef pKey)
{
  int idx = super::findKey(pKey);
  if (-1 == idx)
    fatal(fatal_no_such_value) << pKey;
  return super::m_Buckets[idx].entry->value();
}

const Value& Object::get(StringRef pKey) const
{
  int idx = super::findKey(pKey);
  if (-1 == idx)
    fatal(fatal_no_such_value) << pKey;
  return super::m_Buckets[idx].entry->value();
}

Value& Object::operator[](StringRef pKey)
{
  bool exist = false;
  StringMap<Value>::entry_type* entry = StringMap<Value>::insert(pKey, exist);
  return entry->value();
}

const Value& Object::operator[](StringRef pKey) const
{
  return get(pKey);
}

bool Object::insert(StringRef pKey, const Value& pValue)
{
  bool exist = false;
  StringMap<Value>::entry_type* entry = StringMap<Value>::insert(pKey, exist);
  if (!exist)
    entry->setValue(pValue);
  return !exist;
}

bool Object::write(StringRef pKey, const Value& pValue)
{
  bool exist = false;
  StringMap<Value>::entry_type* entry = StringMap<Value>::insert(pKey, exist);
  if (exist) //< clean up memory
    entry->value().clear();
  entry->setValue(pValue);
  return !exist;
}

void Object::print(IndentOStream& pOS) const
{
  pOS << "{\n" << std::indent;
  onnc::json::Object::const_iterator e = begin(), eEnd = end();
  while (e != eEnd) {
    pOS << '"' << e->key() << "\": ";
    e->value().print(pOS);
    if (++e != eEnd)
      pOS << ",";
    pOS << '\n';
  }

  pOS << std::unindent << "}";
}
