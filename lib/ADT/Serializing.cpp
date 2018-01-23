//===- Serializing.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/Serializing.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// const char*
//===----------------------------------------------------------------------===//
unsigned int onnc::ByteSize(char* pData)
{
  if (nullptr == pData)
    return 0;
  return (::strlen(pData) + 1);
}

unsigned int onnc::ByteSize(const char* pData)
{
  if (nullptr == pData)
    return 0;
  return (::strlen(pData) + 1);
}

unsigned int onnc::Serialize(void* pDst, char* pSrc)
{
  if (nullptr == pSrc)
    return 0;

  ::strcpy((char*)pDst, (const char*)pSrc);
  return (::strlen((const char*)pDst) + 1);
}

unsigned int onnc::Serialize(void* pDst, const char* pSrc)
{
  if (nullptr == pSrc)
    return 0;

  ::strcpy((char*)pDst, pSrc);
  return (::strlen((const char*)pDst) + 1);
}

unsigned int onnc::Deserialize(char*& pDst, const void* pSrc)
{
  if (nullptr == pSrc)
    return 0;

  pDst = ::strdup((const char*)pSrc);
  return (::strlen(pDst) + 1);
}

//===----------------------------------------------------------------------===//
// std::string
//===----------------------------------------------------------------------===//
unsigned int onnc::ByteSize(const std::string& pData)
{
  return (pData.size() + 1);
}

unsigned int onnc::Serialize(void* pDst, const std::string& pSrc)
{
  ::strcpy((char*)pDst, pSrc.c_str());
  return ::strlen((char*)pDst) + 1;
}

unsigned int onnc::Deserialize(std::string& pDst, const void* pSrc)
{
  pDst = (const char*)pSrc;
  return pDst.size() + 1;
}

//===----------------------------------------------------------------------===//
// StringRef
//===----------------------------------------------------------------------===//
unsigned int onnc::ByteSize(const StringRef& pData)
{
  return (pData.size() + 1);
}

unsigned int onnc::Serialize(void* pDst, const StringRef& pSrc)
{
  ::strcpy((char*)pDst, pSrc.c_str());
  return ::strlen((char*)pDst) + 1;
}

unsigned int onnc::Deserialize(StringRef& pDst, const void* pSrc)
{
  pDst = (const char*)pSrc;
  return pDst.size() + 1;
}

//===----------------------------------------------------------------------===//
// CArgu
//===----------------------------------------------------------------------===//
unsigned int onnc::ByteSize(const CArgu& pArgu)
{
  unsigned int size = sizeof(unsigned int);
  CArgu::const_iterator argu, aEnd = pArgu.end();
  for (argu = pArgu.begin(); argu != aEnd; ++argu) {
    if (nullptr == argu)
      size += 1;
    else
      size += (::strlen(*argu) + 1);
  }
  return size;
}

unsigned int onnc::Serialize(void* pDst, const CArgu& pArgu)
{
  // serialization
  uint32_t num = pArgu.size();
  ::memcpy(pDst, &num, sizeof(uint32_t));

  char* current = (char*)pDst + sizeof(uint32_t);
  CArgu::const_iterator argu, aEnd = pArgu.end();
  for (argu = pArgu.begin(); argu != aEnd; ++argu) {
    if (nullptr == argu) {
      *current = '\0';
      current += 1;
    }
    else {
      ::memcpy((void*)current, *argu, ::strlen(*argu) + 1);
      current += (::strlen(*argu) + 1);
    }
  }
  return (current - (char*)pDst);
}

/// Read memory from address @ref pStart to @ref pArgu
/// @param[out] pArgu The c-style argument being deserialized
/// @return The byte size of parsing.
unsigned int onnc::Deserialize(CArgu& pDst, const void* pSrc)
{
  uint32_t* num = (uint32_t*)pSrc;
  unsigned int size = sizeof(uint32_t);
  for (int i = 0; i < *num; ++i) {
    if ('\0' == *((const char*)pSrc + size)) {
      pDst.push_back(nullptr);
      size += 1;
    }
    else {
      pDst.push_back((const char*)pSrc + size);
      size += (::strlen((const char*)pSrc + size) + 1);
    }
  } // end of for
  return size;
}

//===----------------------------------------------------------------------===//
// CArguList
//===----------------------------------------------------------------------===//
unsigned int onnc::ByteSize(const CArguList& pData)
{
  unsigned int rst = sizeof(uint32_t); //< number of arguments
  CArguList::const_iterator argu, aEnd = pData.end();
  for (argu = pData.begin(); argu != aEnd; ++argu) {
    rst += ByteSize(*argu);
  }
  return rst;
}

unsigned int onnc::Serialize(void* pDst, const CArguList& pSrc)
{
  *((uint32_t*)pDst) = pSrc.size();
  char* current = (char*)pDst;
  current += sizeof(uint32_t);
  CArguList::const_iterator argu, aEnd = pSrc.end();
  for (argu = pSrc.begin(); argu != aEnd; ++argu) {
    current += Serialize((void*)current, *argu);
  }
  return (current - (char*)pDst);
}

unsigned int onnc::Deserialize(CArguList& pDst, const void* pSrc)
{
  uint32_t* num = (uint32_t*)pSrc;
  const char* current = (const char*)pSrc + sizeof(uint32_t);
  for (int i = 0; i < *num; ++i) {
    CArgu cargu;
    current += Deserialize(cargu, current);
    pDst.push_back(cargu);
  }
  return (current - (const char*)pSrc);
}

//===----------------------------------------------------------------------===//
// CUnit
//===----------------------------------------------------------------------===//
unsigned int onnc::ByteSize(const CUnit& pCUnit)
{
  unsigned int size = sizeof(uint32_t); //< number of features
  /// size of m_ID
  size += ByteSize(pCUnit.id());
  /// size of cassette::Values
  CUnit::const_iterator fiter, fend = pCUnit.cend();
  for (fiter = pCUnit.cbegin(); fiter != fend; fiter++) {
    size += fiter->bytes();
  }
  return size;
}

unsigned int onnc::Serialize(void* pDst, const CUnit& pCUnit)
{
  // serialization
  *((uint32_t*)pDst) = pCUnit.size();
  char* current = (char*)pDst + sizeof(uint32_t);
  current += Serialize(pDst, pCUnit.id()); //< m_ID;

  CUnit::const_iterator fiter, fend = pCUnit.cend();
  for (fiter = pCUnit.cbegin(); fiter != fend; fiter++) {
    current += fiter->serialize(current);
  }
  return (current - (char*)pDst);
}

unsigned int onnc::Deserialize(CUnit& pDst, const void* pSrc)
{
  uint32_t* num = (uint32_t*)pSrc;
  unsigned int size = sizeof(uint32_t);
  const char* current = (const char*)pSrc + sizeof(uint32_t);
  StringRef id;
  current += Deserialize(id, current);
  pDst.id() = id;
  for (int i = 0; i < *num; ++i) {
    cassette::Value value;
    current += value.deserialize(current);
    pDst.appendFeature(value);
  } // end of for
  return size;
}

//===----------------------------------------------------------------------===//
// Revision
//===----------------------------------------------------------------------===//
unsigned int onnc::ByteSize(const Revision& pRevision)
{
  unsigned int size = ByteSize(pRevision.option().name());
  size += ByteSize(pRevision.regex());
  size += ByteSize(pRevision.getOutFormat());
  return 0;
}

unsigned int onnc::Serialize(void* pDst, const Revision& pRevision)
{
  char* current = (char*)pDst;
  current += Serialize(pDst, pRevision.option().name());
  current += Serialize(pDst, pRevision.regex());
  current += Serialize(pDst, pRevision.getOutFormat());
  return (current - (char*)pDst);
}

unsigned int onnc::Deserialize(Revision& pDst, const void* pSrc)
{
  // TODO: Revision deserialize.
  return 0;
}
