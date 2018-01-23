//===- CEnv.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/CEnv.h>
#include <onnc/Support/OStrStream.h>
#include <onnc/Support/Path.h>
#include <onnc/Support/Environ.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Local functions
//===----------------------------------------------------------------------===//
static inline void clone(CEnv::EnvMap& pTo, const CEnv::EnvMap& pFrom)
{
  CEnv::const_iterator it, itEnd = pFrom.end();
  for (it = pFrom.begin(); it != itEnd; ++it) {
    bool exist = false;
    CEnv::EnvMap::entry_type* entry = pTo.insert(it->key(), exist);
    entry->setValue(::strdup(it->value().c_str()));
  }
}

//===----------------------------------------------------------------------===//
// CEnv
//===----------------------------------------------------------------------===//
CEnv::CEnv()
  : m_EnvMap() {
}

CEnv::CEnv(const CEnv& pCopy)
{
  clone(m_EnvMap, pCopy.m_EnvMap);
}

CEnv::CEnv(const Environ& pHost)
  : m_EnvMap() {
  Environ::iterator var, vEnd = pHost.end();
  for (var = pHost.begin(); var != vEnd; ++var) {
    this->set(var.key().str(), var.value().str());
  }
}

CEnv& CEnv::operator=(const CEnv& pCopy)
{
  clone(m_EnvMap, pCopy.m_EnvMap);
  return *this;
}

CEnv::~CEnv()
{
  iterator it, itEnd = end();
  for (it = begin(); it != itEnd; ++it) {
    if (it->value().isValid())
      ::free((void*)it->value().data());
  }
}

CEnv& CEnv::set(const std::string& pName, const std::string& pValue)
{
  bool exist = false;
  EnvMap::entry_type* entry = m_EnvMap.insert(pName, exist);
  if (exist)
    ::free((void*)entry->value().data());
  entry->setValue(::strdup(pValue.c_str()));

  return *this;
}

CEnv& CEnv::unset(const std::string& pName)
{
  m_EnvMap.erase(pName);
  return *this;
}

StringRef CEnv::get(const std::string& pName) const
{
  EnvMap::const_iterator it = m_EnvMap.find(pName);
  if (it == m_EnvMap.end())
    return StringRef();
  return it->value();
}

bool CEnv::has(const std::string& pName) const
{
  EnvMap::const_iterator it = m_EnvMap.find(pName);
  return (it != m_EnvMap.end());
}

void CEnv::make(char**& pEnvp) const
{
  const unsigned int n = size();
  pEnvp = (char**)malloc(sizeof(char*) * (n + 1));
  pEnvp[n] = nullptr;

  std::string str;
  OStrStream oss(str);

  unsigned int i = 0;
  CEnv::const_iterator it, itEnd = end();
  for (it = begin(); it != itEnd; ++it, ++i) {
    oss.clearbuf();
    oss << it->key() << '=' << it->value();
    pEnvp[i] = ::strdup(str.c_str());
  }
}

void CEnv::free(char**& pEnvp)
{
  for (unsigned int i = 0; nullptr != pEnvp[i]; ++i) {
    ::free(pEnvp[i]);
  }
  ::free(pEnvp);
  pEnvp = nullptr;
}

void CEnv::print(std::ostream& pOS) const
{
  const_iterator it, itEnd = end();
  for (it = begin(); it != itEnd; ++it) {
    pOS << it->key() << '=' << it->value() << ";\n";
  }
}
