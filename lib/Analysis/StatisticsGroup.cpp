//===- StatisticsGroup.cpp -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/StatisticsGroup.h>
#include <onnc/Analysis/Statistics.h>
#include <onnc/Support/ManagedStatic.h>
#include <onnc/Diagnostic/MsgHandling.h>

using namespace onnc;

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

//===----------------------------------------------------------------------===//
// StatisticsGroup::GroupIterator
//===----------------------------------------------------------------------===//
StatisticsGroup::GroupIterator::GroupIterator()
  : m_pObject(NULL) {
}

StatisticsGroup::GroupIterator::GroupIterator(const json::Object::iterator& pPtr,
                                             json::Object& pObject)
  : m_Ptr(pPtr), m_pObject(&pObject) {
}

StatisticsGroup::GroupIterator& StatisticsGroup::GroupIterator::next()
{
  if (NULL == m_pObject)
    return *this;

  // go to the next group
  do {
    ++m_Ptr;
  } while (m_Ptr != m_pObject->end() && !CheckGroup(m_Ptr->value()));

  return *this;
}

StringRef StatisticsGroup::GroupIterator::name() const
{
  return m_Ptr->key();
}

StatisticsGroup StatisticsGroup::GroupIterator::group()
{
  return StatisticsGroup(m_Ptr->value().asObject());
}

//===----------------------------------------------------------------------===//
// StatisticsGroup::ValueIterator
//===----------------------------------------------------------------------===//
StatisticsGroup::ValueIterator::ValueIterator()
  : m_pArray(nullptr) {
}

StatisticsGroup::ValueIterator::ValueIterator(const json::Array::iterator& pPtr,
                                             json::Array& pArray)
  : m_Ptr(pPtr), m_pArray(&pArray) {
}

StatisticsGroup::ValueIterator& StatisticsGroup::ValueIterator::next()
{
  if (nullptr != m_pArray) {
    // go to the next group
    do {
      ++m_Ptr;
    } while (m_Ptr != m_pArray->end() && !CheckGroup(*m_Ptr));
  }

  return *this;
}

StatisticsGroup StatisticsGroup::ValueIterator::group()
{
  return StatisticsGroup(m_Ptr->asObject());
}

//===----------------------------------------------------------------------===//
// StatisticsGroup
//===----------------------------------------------------------------------===//
StatisticsGroup::StatisticsGroup(Statistics& pParent, StringRef pName)
  : m_pObject(pParent.group(pName).m_pObject) {
}

StatisticsGroup::StatisticsGroup(json::Object& pObject)
  : m_pObject(&pObject) {
}

StatisticsGroup StatisticsGroup::group(StringRef pName)
{
  if (hasGroup(pName))
    return StatisticsGroup((*m_pObject)[pName].asObject());
  return Null();
}

bool StatisticsGroup::hasGroup(StringRef pGroup) const
{
  return CheckGroup(*m_pObject, pGroup);
}

bool StatisticsGroup::deleteGroup(StringRef pName)
{
  if (!hasGroup(pName))
    return false;

  m_pObject->erase(pName);
  return true;
}

StatisticsGroup StatisticsGroup::addGroup(StringRef pName, bool* pExist)
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

StringList StatisticsGroup::groupList() const
{
  StringList result;
  groupList(result);
  return result;
}

bool StatisticsGroup::hasEntry(StringRef pName) const
{
  return m_pObject->hasValue(pName);
}

StringList StatisticsGroup::entryList() const
{
  StringList result;
  entryList(result);
  return result;
}

void StatisticsGroup::clear()
{
  m_pObject->clear();
}

bool StatisticsGroup::isEmpty() const
{
  return (m_pObject->begin() == m_pObject->end());
}

bool StatisticsGroup::isNull() const
{
  return (m_pObject == &(*g_NullObject));
}

StatisticsGroup StatisticsGroup::Null()
{
  return StatisticsGroup(*g_NullObject);
}

#define READ_ENTRY(TYPE, NAME) \
TYPE StatisticsGroup::readEntry(StringRef pKey, TYPE pDefault) const \
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

StringRef StatisticsGroup::readEntry(StringRef pKey, const char* pDefault) const
{
  return readEntry(pKey, StringRef(pDefault));
}

StringRef StatisticsGroup::readEntry(StringRef pKey, const std::string& pDefault) const
{
  return readEntry(pKey, pDefault.c_str());
}

Path StatisticsGroup::readEntry(StringRef pKey, const Path& pDefault) const
{
  return Path(this->readEntry(pKey, pDefault.native().c_str()));
}

StringList StatisticsGroup::readEntry(StringRef pKey, const StringList& pDefault) const
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
StatisticsGroup::readEntry(StringRef pKey, const std::vector<TYPE>& pDefault) const \
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

void StatisticsGroup::writeEntry(StringRef pKey, StringRef pValue)
{
  m_pObject->write(pKey, pValue);
}

void StatisticsGroup::writeEntry(StringRef pKey, const Path& pValue)
{
  m_pObject->write(pKey, pValue.native());
}

void StatisticsGroup::groupList(StringList& pGrpNames) const
{
  json::Object::iterator val, vEnd = m_pObject->end();
  for (val = m_pObject->begin(); val != vEnd; ++val) {
    if (CheckGroup(val->value()))
      pGrpNames.push_back(val->key());
  }
}

void StatisticsGroup::entryList(StringList& pEntryNames) const
{
  json::Object::iterator val, vEnd = m_pObject->end();
  for (val = m_pObject->begin(); val != vEnd; ++val) {
    if (!CheckGroup(val->value()))
      pEntryNames.push_back(val->key());
  }
}

StatisticsGroup::GroupIterator StatisticsGroup::gBegin()
{
  // go to the first group.
  json::Object::iterator val = m_pObject->begin();
  json::Object::iterator vEnd = m_pObject->end();
  while (val != vEnd && !CheckGroup(val->value()))
    ++val;
  return GroupIterator(val, *m_pObject);
}

StatisticsGroup::GroupIterator StatisticsGroup::gEnd()
{
  return GroupIterator(m_pObject->end(), *m_pObject);
}

StatisticsGroup::ValueIterator StatisticsGroup::vBegin(StringRef pName)
{
  if (!CheckList(*m_pObject, pName))
    return ValueIterator();
  json::Array& val = (*m_pObject)[pName].asArray();
  return ValueIterator(val.begin(), val);
}

StatisticsGroup::ValueIterator StatisticsGroup::vEnd(StringRef pName)
{
  if (!CheckList(*m_pObject, pName))
    return ValueIterator();
  json::Array& val = (*m_pObject)[pName].asArray();
  return ValueIterator(val.end(), val);
}
