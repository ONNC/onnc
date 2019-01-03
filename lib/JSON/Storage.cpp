//===- Storage.cpp --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/JSON/Storage.h>
#include <onnc/JSON/Group.h>
#include <onnc/Support/IndentOStream.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/JSON/Reader.h>
#include <fstream>
#include <string>

namespace onnc {
namespace json {

//===----------------------------------------------------------------------===//
// Storage
//===----------------------------------------------------------------------===//
// Storage hides the implementation details to keep flexibility for change.
// It is reduced to an adapter of StoragePrivate.
//===----------------------------------------------------------------------===//
Storage::Storage()
  : m_pGroup(NULL), m_AccessMode(kReadOnly), m_FilePath(), m_Value() {
  read("{ }");
}

Storage::Storage(int)
  : m_pGroup(NULL), m_AccessMode(kReadOnly), m_FilePath(), m_Value() {
}

Storage::Storage(StringRef pContent)
  : m_pGroup(NULL), m_AccessMode(kReadOnly), m_FilePath(), m_Value() {
  read(pContent);
}

Storage::Storage(const std::string& pContent)
  : m_pGroup(NULL), m_AccessMode(kReadOnly), m_FilePath(), m_Value() {
  read(StringRef(pContent));
}

Storage::Storage(const char* pContent)
  : m_pGroup(NULL), m_AccessMode(kReadOnly), m_FilePath(), m_Value() {
  if (NULL == pContent)
    error("can not read NULL json string");
  else
    read(StringRef(pContent));
}

Storage::Storage(const Path& pFilePath, AccessMode pMode)
  : m_pGroup(NULL), m_AccessMode(pMode), m_FilePath(pFilePath) {
  open(pFilePath, pMode);
}

Storage::~Storage()
{
  sync();
  delete m_pGroup;
}

bool Storage::isValid() const
{
  return (NULL != m_pGroup);
}

Storage& Storage::open(const Path& pFilePath, AccessMode pMode)
{
  if (!isValid()) {
    m_AccessMode = pMode;
    m_FilePath = pFilePath;

    // read the JSON file
    json::Reader reader;
    json::Reader::Result result = reader.parse(m_FilePath, m_Value);
    // can not open file
    if (json::Reader::kCantOpen == result ) {
      error("can not open configuration file:") << m_FilePath;
      return *this;
    }

    // illegal format
    if (json::Reader::kIllegal == result ||
        (!m_Value.isObject() && !m_Value.isUndefined())) {
      error("can not parse configuration file:") << m_FilePath;
      return *this;
    }

    // empty file
    if (m_Value.isUndefined()) {
      json::Object* object = new json::Object();
      m_Value.delegate(*object);
    }

    // create group
    m_pGroup = new Group(m_Value.asObject());
  }
  return *this;
}

Storage& Storage::read(StringRef pContent)
{
  if (pContent.isValid() && !isValid()) {
    m_AccessMode = kReadOnly;

    // reset configuration object
    m_FilePath.clear();
    m_Value.clear();

    // read the JSON content
    json::Reader reader;
    if (!reader.read(pContent, m_Value) || !m_Value.isObject()) {
      error("can not read json string:") <<  pContent;
    }
    else {
      // create group
      m_pGroup = new Group(m_Value.asObject());
    }
  }
  return *this;
}

Storage::AccessMode Storage::accessMode() const
{
  return m_AccessMode;
}

StringList Storage::groupList() const
{
  StringList result;
  m_pGroup->groupList(result);
  return result;
}

void Storage::groupList(StringList& pGList) const
{
  m_pGroup->groupList(pGList);
}

bool Storage::hasGroup(StringRef pGroup) const
{
  return m_pGroup->hasGroup(pGroup);
}

bool Storage::deleteGroup(StringRef pGroup)
{
  return m_pGroup->deleteGroup(pGroup);
}

const Group Storage::group(StringRef pGroup) const
{
  return m_pGroup->group(pGroup);
}

Group Storage::group(StringRef pGroup)
{
  return m_pGroup->group(pGroup);
}

Group Storage::addGroup(StringRef pName)
{
  m_Value.insert(pName, json::Object());
  return this->group(pName);
}

Storage& Storage::update(StringRef pName, const Group& pGroup)
{
  m_Value.write(pName, json::Object(*pGroup.m_pObject));
  return *this;
}

Storage& Storage::merge(StringRef pName, const Group& pGroup)
{
  m_Value.insert(pName, json::Object(*pGroup.m_pObject));
  return *this;
}

void Storage::print(std::ostream& pOS) const
{
  IndentOStream ios(pOS, 2);
  m_Value.print(ios);
}

Storage& Storage::reset()
{
  delete m_pGroup;
  m_pGroup = NULL;
  m_AccessMode = kReadOnly;
  m_FilePath.clear();
  m_Value.clear();
  return *this;
}

Group Storage::top()
{
  return Group(m_Value.asObject());
}

bool Storage::sync()
{
  if (kReadWrite == accessMode() && is_regular(m_FilePath)) {
    std::ofstream ofs(m_FilePath.c_str());
    if (!ofs.is_open())
      return false;

    IndentOStream ios(ofs, 2);
    m_Value.print(ios);
  }
  return true;
}

} // namespace of json
} // namespace of onnc
