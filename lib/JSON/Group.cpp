//===- Group.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/JSON/Group.h>
#include <onnc/JSON/Storage.h>
#include <onnc/Support/ManagedStatic.h>
#include <onnc/Diagnostic/MsgHandling.h>

namespace onnc {

static ManagedStatic<json::Object> g_NullObject;

//===----------------------------------------------------------------------===//
// Non-member function
//===----------------------------------------------------------------------===//
/// Check if @ref pValue is a group
static inline bool CheckGroup(const json::Value& pValue)
{
  return pValue.isObject();
}

/// Check if @ref pObject has a sub-group @ref pName.
static inline bool CheckGroup(const json::Object& pObject, StringRef pName)
{
  if (!pObject.hasValue(pName))
    return false;

  return CheckGroup(pObject.get(pName));
}

static inline bool CheckList(const json::Object& pObject, StringRef pName)
{
  if (!pObject.hasValue(pName))
    return false;

  return pObject.get(pName).isArray();
}

namespace json {

//===----------------------------------------------------------------------===//
// Group::GroupIterator
//===----------------------------------------------------------------------===//
Group::GroupIterator::GroupIterator()
  : m_pObject(NULL) {
}

Group::GroupIterator::GroupIterator(const json::Object::iterator& pPtr,
                                             json::Object& pObject)
  : m_Ptr(pPtr), m_pObject(&pObject) {
}

Group::GroupIterator& Group::GroupIterator::next()
{
  if (NULL == m_pObject)
    return *this;

  // go to the next group
  do {
    ++m_Ptr;
  } while (m_Ptr != m_pObject->end() && !CheckGroup(m_Ptr->value()));

  return *this;
}

StringRef Group::GroupIterator::name() const
{
  return m_Ptr->key();
}

Group Group::GroupIterator::group()
{
  return Group(m_Ptr->value().asObject());
}

//===----------------------------------------------------------------------===//
// Group::ValueIterator
//===----------------------------------------------------------------------===//
Group::ValueIterator::ValueIterator()
  : m_pArray(nullptr) {
}

Group::ValueIterator::ValueIterator(const json::Array::iterator& pPtr,
                                             json::Array& pArray)
  : m_Ptr(pPtr), m_pArray(&pArray) {
}

Group::ValueIterator& Group::ValueIterator::next()
{
  if (nullptr != m_pArray) {
    // go to the next group
    do {
      ++m_Ptr;
    } while (m_Ptr != m_pArray->end() && !CheckGroup(*m_Ptr));
  }

  return *this;
}

Group Group::ValueIterator::group()
{
  return Group(m_Ptr->asObject());
}

//===----------------------------------------------------------------------===//
// Group
//===----------------------------------------------------------------------===//
Group::Group()
  : m_pObject(&*g_NullObject) {
}

Group::Group(Storage& pParent, StringRef pName)
  : m_pObject(pParent.group(pName).m_pObject) {
}

Group::Group(json::Object& pObject)
  : m_pObject(&pObject) {
}

Group::Group(const Group& pOther)
  : m_pObject(pOther.m_pObject) {
}

Group& Group::operator=(const Group& pOther)
{
  m_pObject = pOther.m_pObject;
  return *this;
}

Group Group::group(StringRef pName)
{
  if (hasGroup(pName))
    return Group((*m_pObject)[pName].asObject());
  return Null();
}

bool Group::hasGroup(StringRef pGroup) const
{
  return CheckGroup(*m_pObject, pGroup);
}

bool Group::deleteGroup(StringRef pName)
{
  if (!hasGroup(pName))
    return false;

  m_pObject->erase(pName);
  return true;
}

Group Group::addGroup(StringRef pName, bool* pExist)
{
  if (hasGroup(pName)) {
    if (nullptr != pExist)
      *pExist = true;
    return this->group(pName);
  }

  if (nullptr != pExist)
    *pExist = false;
  m_pObject->insert(pName, json::Value(json::Object()));
  return this->group(pName);
}

StringList Group::groupList() const
{
  StringList result;
  groupList(result);
  return result;
}

bool Group::hasEntry(StringRef pName) const
{
  return m_pObject->hasValue(pName);
}

StringList Group::entryList() const
{
  StringList result;
  entryList(result);
  return result;
}

void Group::clear()
{
  m_pObject->clear();
}

bool Group::isEmpty() const
{
  return (m_pObject->begin() == m_pObject->end());
}

bool Group::isNull() const
{
  return (m_pObject == &(*g_NullObject));
}

Group Group::Null()
{
  return Group(*g_NullObject);
}

#define READ_ENTRY(TYPE, NAME) \
TYPE Group::readEntry(StringRef pKey, TYPE pDefault) const \
{ \
  if (!m_pObject->hasValue(pKey) || !(*m_pObject)[pKey].is##NAME()) \
    return pDefault; \
  return m_pObject->get(pKey).to##NAME(); \
}

READ_ENTRY(int,           Integer)
READ_ENTRY(unsigned int,  Integer)
READ_ENTRY(StringRef,     String)
READ_ENTRY(long,          Integer)
READ_ENTRY(double,        Floating)
READ_ENTRY(bool,          Bool)

StringRef Group::readEntry(StringRef pKey, const char* pDefault) const
{
  return readEntry(pKey, StringRef(pDefault));
}

StringRef Group::readEntry(StringRef pKey, const std::string& pDefault) const
{
  return readEntry(pKey, pDefault.c_str());
}

Path Group::readEntry(StringRef pKey, const Path& pDefault) const
{
  return Path(this->readEntry(pKey, pDefault.native().c_str()));
}

StringList Group::readEntry(StringRef pKey, const StringList& pDefault) const
{
  if (!CheckList(*m_pObject, pKey))
    return pDefault; // cause a copy

  StringList result;
  json::Array& list = (*m_pObject)[pKey].asArray();
  json::Array::iterator value, vEnd = list.end();
  for (value = list.begin(); value != vEnd; ++value) {
    if (!value->isString())
      return pDefault;
    result.push_back(value->asString());
  }
  return result;
}

#define READ_LIST(TYPE, NAME) std::vector<TYPE> \
Group::readEntry(StringRef pKey, const std::vector<TYPE>& pDefault) const \
{ \
  if (!CheckList(*m_pObject, pKey)) \
    return pDefault; \
\
  std::vector<TYPE> result; \
  json::Array& list = (*m_pObject)[pKey].asArray(); \
  json::Array::iterator value, vEnd = list.end(); \
  for (value = list.begin(); value != vEnd; ++value) { \
    if (!value->is##NAME()) \
      return pDefault; \
    result.push_back(value->to##NAME()); \
  } \
  return result; \
}

READ_LIST(int,    Integer)
READ_LIST(long,   Integer)
READ_LIST(float,  Floating)
READ_LIST(double, Floating)
READ_LIST(bool,   Bool)

void Group::writeEntry(StringRef pKey, StringRef pValue)
{
  m_pObject->write(pKey, pValue);
}

void Group::writeEntry(StringRef pKey, const Path& pValue)
{
  m_pObject->write(pKey, pValue.native());
}

void Group::groupList(StringList& pGrpNames) const
{
  json::Object::iterator val, vEnd = m_pObject->end();
  for (val = m_pObject->begin(); val != vEnd; ++val) {
    if (CheckGroup(val->value()))
      pGrpNames.push_back(val->key());
  }
}

void Group::entryList(StringList& pEntryNames) const
{
  json::Object::iterator val, vEnd = m_pObject->end();
  for (val = m_pObject->begin(); val != vEnd; ++val) {
    if (!CheckGroup(val->value()))
      pEntryNames.push_back(val->key());
  }
}

Group::GroupIterator Group::gBegin()
{
  // go to the first group.
  json::Object::iterator val = m_pObject->begin();
  json::Object::iterator vEnd = m_pObject->end();
  while (val != vEnd && !CheckGroup(val->value()))
    ++val;
  return GroupIterator(val, *m_pObject);
}

Group::GroupIterator Group::gEnd()
{
  return GroupIterator(m_pObject->end(), *m_pObject);
}

Group::ValueIterator Group::vBegin(StringRef pName)
{
  if (!CheckList(*m_pObject, pName))
    return ValueIterator();
  json::Array& val = (*m_pObject)[pName].asArray();
  return ValueIterator(val.begin(), val);
}

Group::ValueIterator Group::vEnd(StringRef pName)
{
  if (!CheckList(*m_pObject, pName))
    return ValueIterator();
  json::Array& val = (*m_pObject)[pName].asArray();
  return ValueIterator(val.end(), val);
}

} // namespace of json
} // namespace of onnc
