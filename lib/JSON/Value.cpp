//===- Value.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/JSON/Value.h>
#include <onnc/JSON/Object.h>
#include <onnc/JSON/Array.h>
#include <cstring>
#include <cstdlib>

using namespace onnc;
using namespace onnc::json;

//===----------------------------------------------------------------------===//
// Value
//===----------------------------------------------------------------------===//
Value::Value()
  : Notation(Type::UNDEF), m_Value() {
}

Value::Value(const Value& pCopy)
  : Notation(pCopy) {
  switch (this->type()) {
    case INT:
      m_Value.int_p = pCopy.m_Value.int_p;
      break;
    case FLOAT:
      m_Value.float_p = pCopy.m_Value.float_p;
      break;
    case BOOL:
      m_Value.bool_p = pCopy.m_Value.bool_p;
      break;
    case STRING:
      m_Value.string_p = ::strdup(pCopy.m_Value.string_p);
      break;
    case OBJECT:
      m_Value.object_p = new Object(*pCopy.m_Value.object_p);
      break;
    case ARRAY:
      m_Value.array_p = new Array(*pCopy.m_Value.array_p);
      break;
    case NIL:
    case UNDEF:
      break;
  }
}

Value::Value(Value&& pOther)
  : Notation(std::move(pOther)), m_Value() {
  m_Value.pointer = pOther.m_Value.pointer;
  pOther.m_Value.reset();
}

Value::~Value()
{
  clear();
}

Value::Value(long long int pN)
  : Notation(INT) {
  m_Value.int_p = pN;
}

Value::Value(long int pN)
  : Notation(INT) {
  m_Value.int_p = pN;
}

Value::Value(int pN)
  : Notation(INT) {
  m_Value.int_p = pN;
}

Value::Value(unsigned long long int pN)
  : Notation(INT) {
  m_Value.int_p = pN;
}

Value::Value(unsigned long int pN)
  : Notation(INT) {
  m_Value.int_p = pN;
}

Value::Value(unsigned int pN)
  : Notation(INT) {
  m_Value.int_p = pN;
}

Value::Value(long double pF)
  : Notation(FLOAT) {
  m_Value.float_p = pF;
}

Value::Value(double pF)
  : Notation(FLOAT) {
  m_Value.float_p = pF;
}

Value::Value(float pF)
  : Notation(FLOAT) {
  m_Value.float_p = pF;
}

Value::Value(bool pB)
  : Notation(BOOL) {
  m_Value.bool_p = pB;
}

Value::Value(StringRef pS)
  : Notation(STRING) {
  m_Value.string_p = ::strdup(pS.data());
}

Value::Value(const char* pS)
  : Notation(STRING) {
  m_Value.string_p = ::strdup(pS);
}

Value::Value(const Object& pObject)
  : Notation(OBJECT) {
  m_Value.object_p = new Object(pObject);
}

Value::Value(const Array& pArray)
  : Notation(ARRAY) {
  m_Value.array_p = new Array(pArray);
}

Value& Value::operator=(const Value& pCopy)
{
  Notation::operator=(pCopy);
  switch (this->type()) {
    case INT:
      m_Value.int_p = pCopy.m_Value.int_p;
      break;
    case FLOAT:
      m_Value.float_p = pCopy.m_Value.float_p;
      break;
    case BOOL:
      m_Value.bool_p = pCopy.m_Value.bool_p;
      break;
    case STRING:
      m_Value.string_p = ::strdup(pCopy.m_Value.string_p);
      break;
    case OBJECT:
      m_Value.object_p = new Object(*pCopy.m_Value.object_p);
      break;
    case ARRAY:
      m_Value.array_p = new Array(*pCopy.m_Value.array_p);
      break;
    case NIL:
    case UNDEF:
      break;
  }
  return *this;
}

Value& Value::operator=(Value&& pOther)
{
  if (this != &pOther) {
    Notation::operator=(std::move(pOther));
    m_Value.pointer = pOther.m_Value.pointer;
    pOther.m_Value.reset();
  }
  return *this;
}

void Value::print(IndentOStream& pOS) const
{
  switch (this->type()) {
    case INT:
      pOS << m_Value.int_p;
      break;
    case FLOAT:
      pOS << m_Value.float_p;
      break;
    case BOOL:
      pOS << (m_Value.bool_p? "true": "false");
      break;
    case STRING:
      pOS << '"' << m_Value.string_p << '"';
      break;
    case OBJECT:
      m_Value.object_p->print(pOS);
      break;
    case ARRAY:
      m_Value.array_p->print(pOS);
      break;
    case NIL:
      pOS << "null";
      break;
    case UNDEF:
      break;
  }
}

Value& Value::append(const Value& pValue)
{
  assert(this->isArray() && "Value is not a kind of array");
  m_Value.array_p->push_back(pValue);
  return *this;
}

bool Value::insert(StringRef pKey, const Value& pValue)
{
  assert(this->isObject() && "Value is not a kind of object");
  return m_Value.object_p->insert(pKey, pValue);
}

bool Value::write(StringRef pKey, const Value& pValue)
{
  assert(this->isObject() && "Value is not a kind of object");
  return m_Value.object_p->write(pKey, pValue);
}

Value& Value::assign(long long int pValue)
{
  assert((this->isUndefined() || this->isInteger()) && "Value has been assigned");
  m_Type = INT;
  m_Value.int_p = pValue;
  return *this;
}

Value& Value::assign(long int pValue) { return assign((long long int)pValue); }
Value& Value::assign(int pValue)      { return assign((long long int)pValue); }

Value& Value::assign(long double pValue)
{
  assert((this->isUndefined() || this->isFloating()) && "Value has been assigned");
  m_Type = FLOAT;
  m_Value.float_p = pValue;
  return *this;
}

Value& Value::assign(double pValue) { return assign((long double)pValue); }
Value& Value::assign(float pValue)  { return assign((long double)pValue); }

Value& Value::assign(bool pValue)
{
  assert((this->isUndefined() || this->isBool()) && "Value has been assigned");
  m_Type = BOOL;
  m_Value.bool_p = pValue;
  return *this;
}

Value& Value::assign(StringRef pValue)
{
  assert((this->isUndefined() || this->isString()) && "Value has been assigned");
  m_Type = STRING;
  m_Value.string_p = ::strdup(pValue.data());
  return *this;
}

Value& Value::assign(const char* pValue)
{
  assert((this->isUndefined() || this->isString()) && "Value has been assigned");
  m_Type = STRING;
  m_Value.string_p = ::strdup(pValue);
  return *this;
}

Value& Value::assign(const std::string& pValue)
{
  assert((this->isUndefined() || this->isString()) && "Value has been assigned");
  m_Type = STRING;
  m_Value.string_p = ::strdup(pValue.c_str());
  return *this;
}

Value& Value::delegate(Object& pObject)
{
  assert(this->isUndefined() && "Value has been assigned");
  m_Type = OBJECT;
  m_Value.object_p = &pObject;
  return *this;
}

Value& Value::delegate(Array& pArray)
{
  assert(this->isUndefined() && "Value has been assigned");
  m_Type = ARRAY;
  m_Value.array_p = &pArray;
  return *this;
}

void Value::clear()
{
  switch (this->type()) {
    case STRING:
      ::free(m_Value.string_p);
      break;
    case OBJECT:
      delete m_Value.object_p;
      break;
    case ARRAY:
      delete m_Value.array_p;
      break;
    default:
      break;
  }
  m_Value.reset();
  setType(UNDEF);
}
